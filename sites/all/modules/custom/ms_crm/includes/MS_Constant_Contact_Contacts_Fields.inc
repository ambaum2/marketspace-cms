<?php
class MS_Constant_Contact_Contacts_Submission {
    public $submission;
    public $node;

    public function __construct($submission = null, $node = null) {
        $this->submission = $submission;
        $this->node = $node;
    }

    /**
     * return email if submission has an email address
     * otherwise return false
     * @param $submission
     * @return bool|string
     */
    static function SubmissionHasEmail($submission) {
        $email = false;
        foreach($submission as $key => $component) {
            if(isset($component['value']['email_addresses'][0]['email_address'])) {
                $email = $component['value']['email_addresses'][0]['email_address'];
            }
        }
        return $email;
    }
    /**
     * parse the submission array and turn it into
     * an object that constant contact can use.
     * max depth two levels
     *
     * @todo should i use recursion - possibly but may be unecessary
     */
     public function GetSubmission() {
        $result = array();
         foreach ($this->submission->data as $cid => $values) {
             foreach ($values['value'] as $delta => $value) {
                 if($this->IsSerialized($value)) {
                     $array_value = unserialize($value);
                     foreach($array_value as $k => $item) {
                         if(is_array($array_value)) {
                             foreach($array_value as $l => $v) {
                                 $result[$delta][$l] = $v; //notice I don't have index $k because constant contact only has two levels to their json request
                             }
                         } else {
                             $result[$delta][$k] = $item;
                         }
                     }
                 } else {
                     $result[$delta] = $value;
                 }
             }
         }
        return $result;
    }

    /**
     * submissions are passed by reference so use this to avoid that
     * plus we cna then recreate the data array of submission
     * escape that
     * @return stdClass
     */
    private function CreateDefaultSubmission() {
        $submission = new stdClass();
        $submission->nid = $this->submission->nid;
        $submission->uid = $this->submission->uid;
        $submission->remote_addr = $this->submission->remote_addr;
        $submission->is_draft = $this->submission->is_draft;

        return $submission;
    }
    /**
     * unserialize submission
     *
     */
    public function GetUnserializedSubmission() {
        $result = $this->CreateDefaultSubmission();
        foreach ($this->submission->data as $cid => $values) {
            foreach ($values['value'] as $delta => $value) {
                if($this->IsSerialized($value)) {
                    $result->data[$cid]['value'][$delta] = unserialize($value);
                } else {
                    $name = $this->GetComponentName($cid, $this->submission->nid);
                    $result->data[$cid]['value'][$name] = $value;
                }
            }
        }
        return $result;
    }
    public function GetSerializedSubmission() {
        $result = $this->CreateDefaultSubmission();
        foreach ($this->submission->data as $cid => $values) {
            foreach ($values['value'] as $delta => $value) {
                if($this->IsSerialized($value)) {
                    $result->data[$cid]['value'][$delta] = serialize($value);
                } else {
                    $name = $this->GetComponentName($cid, $this->submission->nid);
                    $result->data[$cid]['value'][$name] = $value;
                }
            }
        }
        return $result;
    }

    /**
     * checks if a component is a constant contact
     * component
     * @return bool
     */
    public function GetComponentName($cid, $nid) {
        $result = false;
        $component = db_query("
            select c.form_key from mspacecms.webform_component as c
            where c.nid = $nid and c.cid = $cid
        ")->fetchObject();

        return $component->form_key;
    }

    /**
     * geta a single submission value must return a string always
     * or else it causes errors in drupal_write_record
     * around line 7170 $fields[$field] = (string) $fields[$field];
     * @param $value
     * @return string
     */
    public function GetSingleSubmissionValue($value) {
        $delta_key = "";
        $temp_result = "";
        foreach($value as $delta => $field) {
            $delta_key = $delta;
            if(is_array($field)) {
                foreach($field as $key => $v) {
                    $temp_result[] = $v;
                }
            } else { //this concats to handle the situation where you have array('first_name' => 'alan', 'middle_name' => 'matthew') - this should be avoided in practice
                $temp_result .= $field;
            }
        }

        if(is_array($temp_result)) {
            $result[$delta_key] = serialize($temp_result);
        } else {
            $result = $temp_result;
        }
        return $result;
    }



    /**
     * Tests if an input is valid PHP serialized string.
     *
     * Checks if a string is serialized using quick string manipulation
     * to throw out obviously incorrect strings. Unserialize is then run
     * on the string to perform the final verification.
     *
     * Valid serialized forms are the following:
     * <ul>
     * <li>boolean: <code>b:1;</code></li>
     * <li>integer: <code>i:1;</code></li>
     * <li>double: <code>d:0.2;</code></li>
     * <li>string: <code>s:4:"test";</code></li>
     * <li>array: <code>a:3:{i:0;i:1;i:1;i:2;i:2;i:3;}</code></li>
     * <li>object: <code>O:8:"stdClass":0:{}</code></li>
     * <li>null: <code>N;</code></li>
     * </ul>
     *
     * @author		Chris Smith <code+php@chris.cs278.org>
     * @copyright	Copyright (c) 2009 Chris Smith (http://www.cs278.org/)
     * @license		http://sam.zoy.org/wtfpl/ WTFPL
     * @param		string	$value	Value to test for serialized form
     * @param		mixed	$result	Result of unserialize() of the $value
     * @return		boolean			True if $value is serialized data, otherwise false
     */
    public function IsSerialized($value, &$result = null)
    {
        // Bit of a give away this one
        if (!is_string($value))
        {
            return false;
        }

        // Serialized false, return true. unserialize() returns false on an
        // invalid string or it could return false if the string is serialized
        // false, eliminate that possibility.
        if ($value === 'b:0;')
        {
            $result = false;
            return true;
        }

        $length	= strlen($value);
        $end	= '';

        switch ($value[0])
        {
            case 's':
                if ($value[$length - 2] !== '"')
                {
                    return false;
                }
            case 'b':
            case 'i':
            case 'd':
                // This looks odd but it is quicker than isset()ing
                $end .= ';';
            case 'a':
            case 'O':
                $end .= '}';

                if ($value[1] !== ':')
                {
                    return false;
                }

                switch ($value[2])
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                        break;

                    default:
                        return false;
                }
            case 'N':
                $end .= ';';

                if ($value[$length - 1] !== $end[0])
                {
                    return false;
                }
                break;

            default:
                return false;
        }

        if (($result = @unserialize($value)) === false)
        {
            $result = null;
            return false;
        }
        return true;
    }

}
class MS_Constant_Contact_Contacts_Fields {

    public $form;
    public $form_state;
    public $fieldType;
    public $namespace;
    public $component; /** @var $component array */

    function __construct()
    {
        $a = func_get_args();
        $i = func_num_args();
        if (method_exists($this,$f='__construct'.$i)) {
            call_user_func_array(array($this,$f),$a);
        }
    }

    /**
     * call if component does not exist
     * @todo I'm not sure if this is ever actually needed
     * @param null $key
     * @internal param null $component
     */
    function __construct1($key) {
        if(!empty($key)) {
            $this->namespace = $key;
        }
        if (!empty($component)) {
            $this->component = $component;
        }

        $this->InitializeSettingsField($this->namespace);
    }
    /**
     * call if component already exists
     * @param null $key
     * @param null $component
     */
    function __construct2($key, $component) {
        if(!empty($key)) {
            $this->namespace = $key;
        }
        if (!empty($component)) {
            $this->component = $component;
        }

        $this->InitializeSettingsField($this->namespace, $this->component);
    }

    /**
     * call if component needs form and form state.
     * usually useful for ajax or default value processing
     * @param $key
     * @param $component
     * @param $form_state
     */
    function __construct3($key, $component, $form_state) {
        $this->namespace = $key;
        $this->component = $component;
        $this->form_state = $form_state;
        $this->InitializeSettingsField($this->namespace, $this->component);
    }

    /**
     * initializes the constructors
     */
    private function InitializeSettingsField($key, $component) {
        if(!empty($key) && class_exists(trim(strtolower("MS_CC_Settings_Field_" . $key)))) {
            $class = trim(strtolower("MS_CC_Settings_Field_" . $key));
            $this->fieldType = new $class($key, $component);
        } else {
            $this->fieldType = new MS_CC_Settings_Field_Text_Field($key, $component);
        }
    }

    /**
     * build a component for ajax requests
     */
    public static function BuildComponent($form_state) {
        $cid = !empty($form_state['values']['cid']) ? $form_state['values']['cid'] : 'new'; //either an id or "new" if new component
        $nid = !empty($form_state['values']['nid']) ? $form_state['values']['nid'] : arg(1);
        $type = !empty($form_state['values']['type']) ? $form_state['values']['type'] : arg(5); //the component type
        if($cid == 'new') { //in ajax $_GET is [q] => system/ajax so no url info will be available to new components. this is unnecessary if the component already exists
            $_GET['pid'] = $form_state['values']['pid'];
            $_GET['weight'] = $form_state['values']['weight'];
            $_GET['mandatory'] = $form_state['values']['mandatory'];
            $_GET['name'] = $form_state['values']['name'];
        }
        $component = webform_menu_component_load($cid, $nid, $type);
        return $component;
    }
    /**
     * checks if a component is a constant contact
     * component
     * @param $cid
     * @param $nid
     * @return bool
     */
    public function IsConstantContactComponent($cid, $nid) {
        $result = false;
        $component = db_query("
            select c.type from mspacecms.webform_component as c
            where c.nid = $nid and c.cid = $cid
        ")->fetchObject();
        if($component->type == 'ms_crm_cc') {
            $result = true;
        }
        return $result;
    }

    static function GetFormFromCache($form_build_id) {
        $result = db_query("select f.data
             from cache_form as f
            where f.cid = 'form_". $form_build_id . "'"
        )->fetchObject();

        return $result->data;
    }
    /**
     * get the settings field default value path - we
     * are coding to a strict implmentation right now so
     * most of the path is hard coded
     * @param $field_key
     * @return mixed|bool
     */
    public function GetSettingsFieldDefault($field_key) {
        $result = false;

        if(isset($this->component['extra']['cc']['settings'][$field_key])) {
            /** @var $result mixed */
            $result = $this->component['extra']['cc']['settings'][$field_key];
        }
        return $result;
    }

    /**
     * set a default settings field value if
     * one is given
     * @param $field_key
     * @param $element
     */
    public function SetSettingsFieldDefault($field_key, $element) {
        if($value = $this->GetSettingsFieldDefault($field_key)) {
            $element[$field_key]['#default_value'] = $value;
        }
        return $element;
    }
    /**
     * @return mixed
     */
    public function GetSettingsForm() {
        return $this->fieldType->GetSettingsFields();
    }

    public function GetSettingsFields(iMS_Constant_Contact_Field $sf) {
        foreach($sf->settingsFields as $key => $field) {
            $getter = 'get' . $field;
            $form[$field] = $sf->{$getter}($field);
            $form = $this->SetSettingsFieldDefault($field, $form);
        }
        return $form;
    }

    /**
     * default implemenation good for linear display
     * fields that have a known amount of possible values
     * @param iMS_Constant_Contact_Field $df
     * @return array
     */
    public function GetDisplayFields(iMS_Constant_Contact_Field $df) {
        $form = array();
        foreach($df->displayFields as $key => $field) {
            $form[$field] = $df->{$field};
            $this->SetDisplayComponentValues($form[$field]);
        }
        return $form;
    }

    protected  function SetDisplayComponentValues($field) {
        $field['#weight'] = $this->component['weight'];
        return $field;
    }
}

interface iMS_Constant_Contact_Field {
    public function GetSettingsFields();
    public function GetDisplayFields();
}

class MS_CC_Settings_Field_Text_Field extends MS_Constant_Contact_Contacts_Fields implements iMS_Constant_Contact_Field {
    public $displayFields = array();
    public $settingsFields = array('maxlength');
    public $settingsMatchup = array();
    public $component;
    public $maxlength;

    /**
     * set max length no params yet
     */
    public function setMaxlength()
    {
        $field = array(
            '#type' => 'textfield',
            '#title' => 'Max Length',
            '#size' => 50,
            '#maxlength' => 5
        );
        $this->maxlength = $field;
    }

    /**
     * @return mixed
     */
    public function getMaxlength()
    {
        return $this->maxlength;
    }

    /**
     * dynamically create the generic text field
     * @param null $field_name
     * @param $component
     */
    public function __construct($field_name, $component) {
        $this->displayFields = array($field_name);
        $this->setMaxlength();
        $this->setSettingsMatchup($field_name);
        $this->component = $component;
    }

    /**
     * @param $field_name
     * @internal param array $settingsMatchup
     */
    public function setSettingsMatchup($field_name)
    {
        $this->settingsMatchup = array($field_name => array('#size'=>'maxlength'));
    } /** @var array map a settings field back to a display field */

    public function GetSettingsFields(iMS_Constant_Contact_Field $field = null) {
        return parent::GetSettingsFields($this);
    }

    public function GetDisplayFields(iMS_Constant_Contact_Field $field = null) {
        $element[$this->component['extra']['field_type']] = array(
            '#type' => 'textfield',
            '#title' => $this->component['name'],
            '#required' => $this->component['mandatory'],
            '#size' => 20,
        );
        $element['#weight'] = $this->component['weight'];
        return $element;
    }
}
class MS_CC_Settings_Field_Email_Addresses extends MS_Constant_Contact_Contacts_Fields implements iMS_Constant_Contact_Field {
    public $displayFields = array();
    public $settingsFields = array('address');
    public $settingsMatchup = array();
    public $component;
    public $address;

    /**
     * dynamically create the generic text field
     * @param null $field_name
     * @param null $component
     */
    public function __construct($field_name, $component) {
        $this->displayFields = array($field_name);
        $this->setSettingsMatchup($field_name);
        $this->component = $component;
        $this->setAddress();
    }

    /**
     * set the address no params yet
     */
    public function setAddress()
    {
        $field = array(
            '#type' => 'container',
            '#title' => 'Email Address- No settings',
            '#size' => 50,
        );
        //$this->address = $field;

    }

    /**
     * @return mixed
     */
    public function getAddress()
    {
        return $this->address;
    }

    /**
     * @param $field_name
     * @internal param array $settingsMatchup
     */
    public function setSettingsMatchup($field_name)
    {
        $this->settingsMatchup = array($field_name => array('#size'=>'maxlength'));
    } /** @var array map a settings field back to a display field */

    public function GetSettingsFields(iMS_Constant_Contact_Field $field = null) {
        return parent::GetSettingsFields($this);
    }

    public function GetDisplayFields(iMS_Constant_Contact_Field $field = null) {
       $element = array();
        $element['email_addresses'][0]['email_address'] = array(
            '#type' => 'textfield',
            '#title' => 'Email Address',
        );
        $element['#weight'] = $this->component['weight'];
        return $element;
    }
}

class MS_CC_Settings_Field_Lists extends MS_Constant_Contact_Contacts_Fields implements iMS_Constant_Contact_Field {
    public $displayFields = array();
    public $settingsFields = array('lists');
    public $settingsMatchup = array();
    public $component;
    public $form_state;
    public $lists;

    /**
     * dynamically create the generic text field
     * @param null $field_name
     * @param null $component
     */
    public function __construct($field_name, $component) {
        $this->displayFields = array($field_name);
        $this->setSettingsMatchup($field_name);
        $this->component = $component;
        $this->setLists();
    }

    /**
     * set lists property
     */
    public function setLists()
    {
        $field = array(
            '#type' => 'select',
            '#multiple' => true,
            '#title' => t('Select Lists'),
            '#required' => true,
            '#options' => array(
                '1966704516' => t("New Local Sellers"),
            ),
        );

        $this->lists = $field;
    }

    /**
     * @return mixed
     */
    public function getLists()
    {
        return $this->lists;
    }

    /**
     * @param $field_name
     * @internal param array $settingsMatchup
     */
    public function setSettingsMatchup($field_name)
    {
        $this->settingsMatchup = array($field_name => array('#size'=>'maxlength'));
    } /** @var array map a settings field back to a display field */

    public function GetSettingsFields(iMS_Constant_Contact_Field $field = null) {
        return parent::GetSettingsFields($this);
    }

    public function GetDisplayFields(iMS_Constant_Contact_Field $field = null) {
        $form = array();

        $field['lists'][0]['id'] = array(
            '#type' => 'hidden',
            '#value' => '1966704516',
            '#weight' => $this->component['weight'],
        );
        /*$field['lists'][1] = array(
            '#type' => 'hidden',
            '#value' => 'hay!',
        );*/
        return $field;
    }
}

class MS_CC_Settings_Field_Addresses extends MS_Constant_Contact_Contacts_Fields implements iMS_Constant_Contact_Field {
    public $displayFields = array('addresses');
    public $settingsFields = array('addressTypes');
    public $settingsMatchup = array();
    public $component;
    public $addressTypes;

    public $addresses = array(
        '#type' => 'container',
        '#title' => 'Addresses',
        '#tree' => true,
    );
    /**
     * dynamically create the generic text field
     * @param null $field_name
     * @param null $component
     */
    public function __construct($field_name, $component) {
        $this->displayFields = array($field_name);
        $this->setSettingsMatchup($field_name);
        $this->component = $component;
        $this->setAddressTypes();
    }

    /**
     * set address property
     */
    public function setAddressTypes()
    {
        $field = array(
            '#type' => 'select',
            '#multiple' => true,
            '#title' => 'Address Types',
            '#options' => array(
                0 => 'Business',
                1 => 'Personal',
            )
        );
        $this->addressTypes = $field;
    }

    /**
     * @return mixed
     */
    public function getAddressTypes()
    {
        return $this->addressTypes;
    }

    /**
     * @param $field_name
     * @internal param array $settingsMatchup
     */
    public function setSettingsMatchup($field_name)
    {
        $this->settingsMatchup = array($field_name => array('#size'=>'maxlength'));
    }

    public function GetSettingsFields(iMS_Constant_Contact_Field $component = null) {

        return parent::GetSettingsFields($this);
    }

    public function GetDisplayFields(iMS_Constant_Contact_Field $component = null) {
        $form = null;
        $address = new MS_CC_Settings_Field_Address();
        $form = $this->addresses;
        $fields = array('address_type', 'city', 'country_code', 'line1','line2', 'line3','postal_code', 'state_code');
        //TODO: multi address ability and get rid of hard code 0 index
        foreach($fields as $key => $field) {
            $getter = str_replace("_", "", "get" . $field);
            $form['addresses'][0][$field] = $address->$getter();
        }
        $form['#weight'] = $this->component['weight'];
        return $form;
    }

    public function GetFormStateValue($value)
    {
        // TODO: Implement GetFormStateValue() method.
    }
}

class MS_CC_Settings_Field_Address extends MS_CC_Settings_Field_Addresses {
    public $address_type;
    public $city;
    public $country_code;
    public $line1;
    public $line2;
    public $line3;
    public $postal_code;
    public $state_code;
    //public $sub_postal_code;

    public function __construct() {

    }

    /**
     * @param mixed $address_type
     */
    public function setAddressType($address_type)
    {
        $this->address_type = $address_type;
    }
    /**
     * @return mixed
     */
    public function getAddressType()
    {
        $this->address_type = array(
            '#type' => 'select',
            '#title' => t('Select Address Type'),
            '#options' => array(
                'BUSINESS' => t('BUSINESS'),
                'PERSONAL' => t('PERSONAL'),
            ),
            //'#default_value' => isset(MS_Constant_Contact_Contacts_Fields::GetFormStateInfo()) ?
            '#description' => t('Set the type of address'),
        );
        return $this->address_type;
    }

    /**
     * @return mixed
     */
    public function getCity()
    {
        $this->city = array(
            '#type' => 'textfield',
            '#title' => t('City'),
            //'#default_value' => $node->title,
            '#size' => 60,
            '#maxlength' => 128,
        );

        return $this->city;
    }

    /**
     * @return mixed
     */
    public function getCountryCode()
    {
        $this->country_code = array(
            '#type' => 'select',
            '#title' => t('Country'),
            '#options' => array('US' => 'United States', 'CA' => 'Canada'),
            //'#default_value' => isset(MS_Constant_Contact_Contacts_Fields::GetFormStateInfo()) ?
            '#description' => t('Set the type of address'),
        );
        return $this->country_code;
    }

    /**
     * @return mixed
     */
    public function getLine1()
    {
        $this->line1 = array(
            '#type' => 'textfield',
            '#title' => t('Address Line 1'),
            //'#default_value' => $node->title,
            '#size' => 60,
            '#maxlength' => 128,
        );
        return $this->line1;
    }

    /**
     * @return mixed
     */
    public function getLine2()
    {
        $this->line2 = array(
            '#type' => 'textfield',
            '#title' => t('Address Line 2'),
            //'#default_value' => $node->title,
            '#size' => 60,
            '#maxlength' => 128,
        );
        return $this->line2;
    }

    /**
     * @return mixed
     */
    public function getLine3()
    {
        $this->line3 = array(
            '#type' => 'textfield',
            '#title' => t('Address Line 3'),
            //'#default_value' => $node->title,
            '#size' => 60,
            '#maxlength' => 128,
        );
        return $this->line3;
    }

    /**
     * @return mixed
     */
    public function getPostalCode()
    {
        $this->postal_code = array(
            '#type' => 'textfield',
            '#title' => t('Zip Code'),
            //'#default_value' => $node->title,
            '#size' => 60,
            '#maxlength' => 5,
        );
        return $this->postal_code;
    }

    /**
     * @return mixed
     */
    public function getStateCode()
    {
        $this->state_code = array(
            '#type' => 'select',
            '#title' => t('Select Address Type'),
            '#options' =>  array(
                'AL' => t('Alabama'),
                'AK' => t('Alaska'),
                'AS' => t('American Samoa'),
                'AZ' => t('Arizona'),
                'AR' => t('Arkansas'),
                'CA' => t('California'),
                'CO' => t('Colorado'),
                'CT' => t('Connecticut'),
                'DE' => t('Delaware'),
                'DC' => t('District of Columbia'),
                'FL' => t('Florida'),
                'GA' => t('Georgia'),
                'GU' => t('Guam'),
                'HI' => t('Hawaii'),
                'ID' => t('Idaho'),
                'IL' => t('Illinois'),
                'IN' => t('Indiana'),
                'IA' => t('Iowa'),
                'KS' => t('Kansas'),
                'KY' => t('Kentucky'),
                'LA' => t('Louisiana'),
                'ME' => t('Maine'),
                'MH' => t('Marshall Islands'),
                'MD' => t('Maryland'),
                'MA' => t('Massachusetts'),
                'MI' => t('Michigan'),
                'MN' => t('Minnesota'),
                'MS' => t('Mississippi'),
                'MO' => t('Missouri'),
                'MT' => t('Montana'),
                'NE' => t('Nebraska'),
                'NV' => t('Nevada'),
                'NH' => t('New Hampshire'),
                'NJ' => t('New Jersey'),
                'NM' => t('New Mexico'),
                'NY' => t('New York'),
                'NC' => t('North Carolina'),
                'ND' => t('North Dakota'),
                'MP' => t('Northern Marianas Islands'),
                'OH' => t('Ohio'),
                'OK' => t('Oklahoma'),
                'OR' => t('Oregon'),
                'PW' => t('Palau'),
                'PA' => t('Pennsylvania'),
                'PR' => t('Puerto Rico'),
                'RI' => t('Rhode Island'),
                'SC' => t('South Carolina'),
                'SD' => t('South Dakota'),
                'TN' => t('Tennessee'),
                'TX' => t('Texas'),
                'UT' => t('Utah'),
                'VT' => t('Vermont'),
                'VI' => t('Virgin Islands'),
                'VA' => t('Virginia'),
                'WA' => t('Washington'),
                'WV' => t('West Virginia'),
                'WI' => t('Wisconsin'),
                'WY' => t('Wyoming'),
            ),
            //'#default_value' => isset(MS_Constant_Contact_Contacts_Fields::GetFormStateInfo()) ?
            '#description' => t('Set the type of address'),
        );

        return $this->state_code;
    }

    /**
     * @return mixed
     */
    public function getSubPostalCode()
    {
        return $this->sub_postal_code;
    }

}
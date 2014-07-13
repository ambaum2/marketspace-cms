<?php
class MS_Constant_Contact_Contacts_Submission {
    public $submission;
    public $node;

    public function __construct($submission, $node) {
        $this->submission = $submission;
        $this->node = $node;
    }

    /**
     * parse the submission array and turn it into
     * an object that constant contact can use.
     * max depth two levels
     *
     * @todo should i use recursion - possibly but may be unecessary
     */
    public function GetSubmissionJson() {
        $result = array();
        foreach ($this->submission->data as $cid => $values) {
            foreach ($values['value'] as $delta => $value) {
                if(is_array($value) && isset($value[''])) {
                    foreach($value as $k => $item) {
                        if(is_array($value)) {
                            foreach($value as $l => $v) {
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
        return webform_menu_component_load($cid, $nid, $type);
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
            $form[$field] = $sf->{$field};
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
        }
        return $form;
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

    /**
     * @param $field_name
     * @internal param array $settingsMatchup
     */
    public function setSettingsMatchup($field_name)
    {
        $this->settingsMatchup = array($field_name => array('#size'=>'maxlength'));
    } /** @var array map a settings field back to a display field */

    /**
     * dynamically create the generic text field
     * @param null $field_name
     * @param $component
     */
    public function __construct($field_name, $component) {
        $this->displayFields = array($field_name);
        $this->setSettingsMatchup($field_name);
        $this->component = $component;
    }


    public $maxlength = array(
        '#type' => 'textfield',
        '#title' => 'Max Length',
        '#size' => 50,
        '#maxlength' => 5
    );

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
         return $element;
    }
}
class MS_CC_Settings_Field_Email_Addresses extends MS_Constant_Contact_Contacts_Fields implements iMS_Constant_Contact_Field {
    public $displayFields = array();
    public $settingsFields = array('address');
    public $settingsMatchup = array();
    public $component;
    /**
     * @param $field_name
     * @internal param array $settingsMatchup
     */
    public function setSettingsMatchup($field_name)
    {
        $this->settingsMatchup = array($field_name => array('#size'=>'maxlength'));
    } /** @var array map a settings field back to a display field */

    /**
     * dynamically create the generic text field
     * @param null $field_name
     * @param null $component
     */
    public function __construct($field_name, $component) {
        $this->displayFields = array($field_name);
        $this->setSettingsMatchup($field_name);
        $this->component = $component;
    }


    public $address = array(
        '#type' => 'textfield',
        '#title' => 'List Id',
        '#size' => 50,
        '#ajax' => array(
            'callback' => 'ms_crm_cc_settings',
            'method' => 'replace',
            'wrapper' => 'cc_field_settings_form',
        ),
    );

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
        return $element;
    }
}

class MS_CC_Settings_Field_Email_Lists extends MS_Constant_Contact_Contacts_Fields implements iMS_Constant_Contact_Field {
    public $displayFields = array();
    public $settingsFields = array('lists');
    public $settingsMatchup = array();
    public $component;
    public $form_state;
    /**
     * @param $field_name
     * @internal param array $settingsMatchup
     */
    public function setSettingsMatchup($field_name)
    {
        $this->settingsMatchup = array($field_name => array('#size'=>'maxlength'));
    } /** @var array map a settings field back to a display field */

    /**
     * dynamically create the generic text field
     * @param null $field_name
     * @param null $component
     */
    public function __construct($field_name, $component) {
        $this->displayFields = array($field_name);
        $this->setSettingsMatchup($field_name);
        $this->component = $component;
    }


    public $lists = array(
        '#type' => 'textfield',
        '#title' => 'List Id',
        '#size' => 50,
        '#ajax' => array(
            'callback' => 'ms_crm_cc_settings',
            'method' => 'replace',
            'wrapper' => 'cc_field_settings_form',
        ),
    );

    /**
     * @param $value
     */
    public function GetFormStateValue($value) {
        // TODO: Implement GetFormStateValue() method.
    }

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
        return $element;
    }
}

class MS_CC_Settings_Field_Addresses extends MS_Constant_Contact_Contacts_Fields implements iMS_Constant_Contact_Field {
    public $displayFields = array('addresses');
    public $settingsFields = array('addressTypes');
    public $settingsMatchup = array();
    public $component;

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
    }

    /**
     * @param $field_name
     * @internal param array $settingsMatchup
     */
    public function setSettingsMatchup($field_name)
    {
        $this->settingsMatchup = array($field_name => array('#size'=>'maxlength'));
    }

    public $addressTypes = array(
        '#type' => 'select',
        '#multiple' => true,
        '#title' => 'Address Types',
        '#options' => array(
            0 => 'Business',
            1 => 'Personal',
        )
   );

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
                0 => t('BUSINESS'),
                1 => t('PERSONAL'),
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
            '#title' => t('Select Address Type'),
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
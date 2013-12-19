<?php
class product {
  
  /**
   * take the array of attributes from magento api method
   * catalogProductAttributeList(sessionId, attribute_set_id)
   *  and return an array
	 * @param attributes | array
	 * 	an array of attributes
   */
  public function create_attribute_select_options($attributes) {
    $options = array();
    foreach($attributes as $item) {
      $options[$item->attribute_id] = $item->code;
    }
    asort($options); //sort alphabetically
    return $options;
  }

  /**
   * create a form input for a given magento 
   * response array
   * @param attribute | array
   * @param form_state | array
   *  this is only used in multipage forms
   *  this is never used in product edit or 
   *  the first page of the form
   * @return array 
   * @TODO : need to take set default value for editing
   * existing product if there is one
   */
  public function create_attribute_input($attribute, $form_state = null, $component) {
	  $form_field = array( //create base field 
	    '#type' => $this->_convert_attribute_type($attribute->frontend_input),
	    '#title' => $attribute->frontend_label,
	    '#required' => (($attribute->is_required == 1) ? TRUE : FALSE),
	    '#description' => isset($component['extra']['description']) ? $component['extra']['description'] : '',
	  );
		
	  if($attribute->frontend_input == "select") { // select is the type for select, multi select, radio etc in magento
    	$form_field['#options'] = $attribute->options;      
	    if($attribute->frontend_input == "multiselect") {
	      $form_field['#multiple'] = TRUE;
	    }
	  }
		  	
    if($attribute->is_wysiwyg_enabled == 1) { //check if this is a wysiwyg editor
  		if(isset($form_state['storage']['submitted'][$component['cid']]['text'][$attribute->frontend_input]['value'])) {
  			$default_text = $form_state['storage']['submitted'][$component['cid']]['text'][$attribute->frontend_input]['value'];
  		}
  		$form_field = array();
  		$form_field['text'] = array( //create the fieldset for wysiwyg
  		  '#type' => 'fieldset',
  		  '#title' => t('Editor'),
  		  '#weight' => 5,
  		  '#collapsible' => TRUE,
  		  '#collapsed' => FALSE,
  		);
      
      $form_field['text'][$attribute->frontend_input] = array( //create wysiwyg editor 
        '#type' => 'text_format', //http://drupal.org/node/1031160#comment-6256154
        '#format' => 'content_admin',
    		'#title' => $attribute->frontend_label,
    		'#required' => (($attribute->is_required == 1) ? TRUE : FALSE),
    		'#description' => isset($component['extra']['description']) ? $component['extra']['description'] : '',
    		array('#attached'=>array('css' => array(drupal_get_path('module', 'es_webform') . '/css/es_webform_wysiwyg.css'))),
  		);
			
  		if(isset($form_state['storage']['submitted'][$component['cid']]['text'][$attribute->frontend_input]['value'])) {
  			$form_field['text'][$attribute->frontend_input]['#default_value'] = $default_text; //set default wysiwyg text
  		}
		}
		return $form_field;
	}

  /*
   * convert magento attribute types to drupal types
   * return the drupal type name if found
   * otherwise default to textfield
   */
  public function _convert_attribute_type($type_name) {
    $types = array(
      "text"=>"textfield",
      "multiselect"=>"select",
      "boolean"=>"select",
      "select"=>"select",
      "weight"=>"textfield",
      "date"=>"date",
      "price"=>"textfield",
      "textarea"=>"textarea",
      );
    $key = array_key_exists($type_name,$types);
    if($key = array_key_exists($type_name,$types)) {
      return $types[$type_name];
    } else {
      return "textfield"; //default to textfield if nothing exists
    }
  }
}
/*
 * prepopulate marketspace wizard
 */
/*
 * /*   if(!isset($form['values']['submitted']) && $form['#is_draft']) {
    $node = $form['#node'];
    $sid = $form['#submission']->sid ? (int) $form['#submission']->sid : NULL;
$submission = webform_get_submission($node->webform['nid'], $sid);
    echo "<pre>some data?" . $node->webform['nid'] . $sid . print_r($submission, true) . "</pre>";
    die;
    // Check if user is submitting as a draft.
    $is_draft = (int) !empty($form_state['save_draft']);
  
    if (!$sid) {
      // Create a new submission object.
      $submission = (object) array(
        'nid' => $node->nid,
        'uid' => $form_state['values']['details']['uid'],
        'submitted' => REQUEST_TIME,
        'remote_addr' => ip_address(),
        'is_draft' => $is_draft,
        'data' => webform_submission_data($node, $form_state['values']['submitted']),
      );
    } else {
      $submission = webform_get_submission($node->webform['nid'], $sid);
      $submission->is_draft = $form['#is_draft'];
      //$node = node_load($form['#submission']->nid);
      // Merge with new submission data. The + operator maintains numeric keys.
      // This maintains existing data with just-submitted data when a user resumes
      // a submission previously saved as a draft.
      /*1. get webform magento attributes if magento attribute code isset for each webform compon
      * $form[#node][webform][components] put them into an array 
       * 2. query magento for those codes
       * 3. 
       */
      //Add your data now create a foreach loop through
/*      $new_data = webform_submission_data($node, $form_state['values']['submitted']);
      $submission->data = $new_data + $submission->data;
  
      // If there is no data to be saved (such as on a multipage form with no fields
      // on the first page), process no further. Submissions with no data cannot
      // be loaded from the database as efficiently, so we don't save them at all.
      if (empty($submission->data)) {
        echo "no data or not formatted properly fail";
        die;
      }
  
      // Save the submission to the database.
      if (!$sid) {
        // No sid was found thus insert it in the dataabase.
        $form_state['values']['details']['sid'] = $sid = webform_submission_insert($node, $submission);
        $form_state['values']['details']['is_new'] = TRUE;
  
        // Set a cookie including the server's submission time.
        // The cookie expires in the length of the interval plus a day to compensate for different timezones.
        if (variable_get('webform_use_cookies', 0)) {
          $cookie_name = 'webform-' . $node->nid;
          $time = REQUEST_TIME;
          $params = session_get_cookie_params();
          setcookie($cookie_name . '[' . $time . ']', $time, $time + $node->webform['submit_interval'] + 86400, $params['path'], $params['domain'], $params['secure'], $params['httponly']);
        }
      } else {
    // Sid was found thus update the existing sid in the database.
    webform_submission_update($node, $submission);
    $form_state['values']['details']['is_new'] = FALSE;
  }
  }
    }
    if(isset($form_state['values']['submitted'])) {

    }
*/
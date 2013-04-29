<?php

/**
 * Contains Webform component callbacks.
 */

/**
 * Implements _webform_defaults_[component]().
 */
function _webform_defaults_es_webform() {
  return array(
    'extra' => array(
      //'payment_currency_code' => 'XXX',
      //'payment_description' => '',
      //'payment_line_items' => array(),
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_es_webform($component) {
  $magento = new magento();
  $product = new product();
  $webform = node_load($component['nid']);
  if(isset($webform->field_attribute_set_id['und'][0]['value'])) {
    $attribute_set_id = $webform->field_attribute_set_id['und'][0]['value'];
    $attributes = $magento->get_all_attributes($attribute_set_id, $magento->decryptText(variable_get('session_id')));
    
    $form['extra']['magento_attribute_code'] = array(
      '#type' => 'select',
      '#title' => t('Magento Attribute'),
      '#options' => $product->create_attribute_select_options($attributes),
      '#default_value' => isset($component['extra']['magento_attribute_code']) ? $component['extra']['magento_attribute_code'] : '',
      '#required' => TRUE,
      '#ajax' => array(
        'callback' => 'get_attribute_info',
        'method' => 'replace',
        'wrapper' => 'edit-extra-attribute-info'//apparenly this has to be the id of the destination element
      )
    );
   $form['extra']['magento_attribute_code_name'] = array(
     '#type' => 'hidden',
     '#prefix' => '<div id="attribute_code_name">',
     '#suffix' => '</div>',
     '#value' => isset($component['extra']['magento_attribute_code_name']) ? $component['extra']['magento_attribute_code_name'] : '1',
   );
   $form['extra']['attribute_info'] = array(
      '#type' => 'item',
      '#title' => t('Attribute Info'),
    );
  return $form;

  } else {
     $form['extra']['warning'] = array(
      '#type' => 'item',
      '#title' => t('Magento Attribute'),
      '#markup' => "<p style='color:red;font-weight:900;'>You must set an attribute set id before you can select an attribute</p>",
    );
	return $form;
  }
}

/*
 * ['extra']['magento_attribute_code']['#value']
 */
function get_attribute_info($form, $form_state) {
  $magento = new magento();
  $attribute = $magento->get_attribute_information($magento->decryptText(variable_get('session_id')), $form['extra']['magento_attribute_code']['#value']);
  ($attribute->is_required == 1 ? $required = 'Yes' : $required = 'No');
  $attribute_code_name = $attribute->code;
  
  //$form['extra']['attribute_info'] = array('#type'=>'item',
  //  '#markup'=>'
  $attribute_info ='<div id="edit-extra-attribute-info"><span>Frontend Name: ' . $attribute->frontend_label[0]->label 
    . '</span><br /><span>Form Type: ' . $attribute->frontend_input . '</span><br /><span>Required: ' 
    . $required . '</div>'; //$attribute->frontend_label[0]->label
  $commands = array();
  //$commands[] = ajax_command_before('edit-extra-attribute-info', $attribute_info);
  $commands[] = ajax_command_replace("#attribute_code_name", '<div id="attribute_code_name"><input type="hidden" value="' . $attribute->attribute_code . '" name="extra[magento_attribute_code_name]"></div>');
  $commands[] = ajax_command_replace("#edit-extra-attribute-info", $attribute_info);
  return array('#type' => 'ajax', '#commands' => $commands);
  //return $form;
}

/**
 * Implements _webform_render_[component]().
 */
function _webform_render_es_webform($component, $value = NULL, $filter = TRUE, $form_state) {
  global $user;
  $magento = new magento();
  $magento_attribute = new magento_product_attributes();
  $product = new product();
  $attribute = $magento_attribute->get_stored_magento_attribute_data($component['extra']['magento_attribute_code']);
  $attribute = unserialize($attribute->data);
  $form = $product->create_attribute_input($attribute, $form_state, $component);
  return $form;
}

function _webform_display_es_webform($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title', 'field_prefix', 'field_suffix'),
  );
}
/*
 * do something before submission is saved. send new product to 
 * magento and try to create. 
 * is_draft will be 0 (not draft anymore). The data array has all the data should
 * look through and if does not have categories key then lookup the key
 * from the categories database and get the attribute code. store
 * attribute_code=> value (values) in an array to be sent to magento
 */
function es_webform_webform_submission_presave($node, &$submission) {
  // Update some component's value before it is saved..
  global $user;
  if($submission->is_draft == 0) {
	  $default_magento_attributes = array('categories','websites','name','description','short_description','weight','status','url_key','url_path'
	    ,'visibility','category_ids','website_ids','has_options','gift_message_available','price','special_price','special_price','special_from_date'
	    ,'special_to_date','tax_class_id','meta_title','meta_keyword','meta_description','custom_design','custom_layout_update','options_container'
	    );
	  $default_magento_stock_attributes = array('qty','is_in_stock','manage_stock','use_config_manage_stock','min_qty','use_config_manage_stock'
	    ,'min_qty','use_config_min_qty','min_sale_qty','use_config_min_sale_qty',' max_sale_qty',' use_config_max_sale_qty','');
  $magento_product_attributes = new magento_product_attributes();
  $magento_additional_attributes = array();
  	$magento_product_data = array();
	   foreach($submission->data as $item=>$value) {
    		if(isset($node->webform['components'][$item]['extra']['magento_attribute_code'])) {
  			$mage_id = $node->webform['components'][$item]['extra']['magento_attribute_code'];
  			  //if it is a standard catalog attribute
  	  		if($attribute_data = $magento_product_attributes->get_stored_magento_attribute_data($mage_id)) {
  				$attribute = unserialize($attribute_data->data);
    	  		if(isset($value['value']['text']['textarea']['value'])) {
    	  		  if(in_array($attribute->attribute_code,$default_magento_attributes)) {
    	  		      $magento_product_data[$attribute->attribute_code] = $value['value']['text']['textarea']['value'];
  	  				} else {
    				      $magento_additional_attributes['single_data'][] = array('key'=>$attribute->attribute_code, 'value'=>$value['value']['text']['textarea']['value']);
  	  				}
    				} else {
    		  				if($attribute->frontend_input == "multiselect") {
      		  				  $selections = array();
                      foreach($value['value'] as $select_key=>$select_value) {
                        $selections[] = $select_value;
                      } 
      		  				  if(in_array($attribute->attribute_code,$default_magento_attributes)) {
        		  					$magento_product_data[$attribute->attribute_code] =	$selections;
                      } else {
                        $magento_additional_attributes['multi_data'][] = array('key'=>$attribute->attribute_code, 'value'=>$selections);
                      }
      						} else {
                      if(in_array($attribute->attribute_code,$default_magento_attributes)) {
        							    $magento_product_data[$attribute->attribute_code] =	$value['value'][0];
                      } else {
                          $magento_additional_attributes['single_data'][] = array('key'=>$attribute->attribute_code, 'value' => $value['value'][0]);
                      }
                  }
    				}
  		//all other attributes must be exlplicitely referenced
  		} //if not then not valid attribute
  	} elseif($item == 23) { //probably want to handle all other types media, categories, custom attributes etc with $node->webform['components'][$item]['extra']['magento_attribute_code_name'] == "categories" or whatever type it is
       $file_fid = file_load($value['value'][0]);
       $file = file_create_url($file_fid->uri);
    } elseif(isset($value['value']['categories'])) {      
        $categories = array();
        foreach($value['value']['categories'] as $cat_key=>$cat_value) {
          if($cat_value == 1) {
            $categories[] = $cat_key;
          }
        }
        $magento_product_data['categories'] = $categories;
    }
	}
  	$magento = new magento();
    $magento_media = new magento_media();
  	$magento_product_data['websites'] = array(1);
  	$magento_product_data['status'] = 1;
  	$magento_product_data['url_key'] = $magento_product_data['name'];
  	$magento_product_data['url_path'] = $magento_product_data['name'];
  	$magento_product_data['visibility'] = 4;
  	$magento_product_data['price'] = 0;
		//TODO: obviously these need to be dynamic 
    $magento_additional_attributes['single_data'][] = array('key' => 'tickettype', 'value' => 34);//$node->field_tickettype['und'][0]['value'];
    $magento_additional_attributes['single_data'][] = array('key' => 'marketspace_owner', 'value' => $user->uid);
    $magento_product_data['additional_attributes'] = $magento_additional_attributes;
    $sku = substr(md5($magento_product_data['name']. time()),0,32);
    //echo "<pre>" . print_r($magento_product_data,true) . "</pre>";
    $product_id = $magento->create_product($magento->decryptText(variable_get('session_id')), 'virtual', $node->field_attribute_set_id['und'][0]['value'], $sku, $magento_product_data);
    $file_output = $magento_media->create_media($magento->decryptText(variable_get('session_id')), $product_id, $magento_product_data, $file, $sku, array('thumbnail','image','small_image'));  
    
  }



//dpm($node);
//$mage_att = new magento_product_attributes();
//$attribute_data = $mage_att->get_stored_magento_attribute_data(198);
// $attribute = unserialize($attribute_data->data);
//dpm($attribute);
}

/*
 * hook webform_component_presave
 * mofdify component before submitted
 * check if magento component (es_webform) if
 * so check database if exists for this nid
 * and cid 
 */
function es_webform_webform_component_presave(&$component) {
  if($component['type'] == "es_webform") { //if this is a magento component
    //check webform_es_attribute for the magento_attribute_code id
    if(isset($component['extra']['magento_attribute_code'])) {
    module_load_include("inc", "es_webform", "includes/magento");
    module_load_include("inc","es_webform", "includes/magento_product_attributes");
    $magento = new magento();
    $magento_attributes = new magento_product_attributes();
    $attribute = $magento->get_attribute_information($magento->decryptText(variable_get('session_id')), $component['extra']['magento_attribute_code']);
      if($magento_attributes->exists_magento_attribute($component['extra']['magento_attribute_code'])) {
        //update
        $magento_attributes->update_magento_attribute($component,serialize($attribute));
      } else {
        //insert
        $magento_attributes->insert_magento_attribute($component,serialize($attribute));
      }
    }
  }
}


/**
 * Implements _webform_submit_[component]().
 * occurs on frontend after user clicks submit or next button
 */
//function _webform_submit_es_webform($component, $value) {
  //es_webform_delete_by_pid($value);
  /*$message = drupal_mail('openlayers', 'notice', "abaum@aaortho.org", language_default(), array(),null, FALSE); //locator@aaomembers.org
  $message['headers']['Content-Type'] = 'text/html; charset=UTF-8; format=flowed';
  $message['subject'] = "LOCATOR IS DOWN: " . strftime("%d/%m/%Y %r", time());
  $message['body'] = "<table><tr><td>Term Search</td><td></td></tr><tr><td>IP Address</td><td>" 
    . $_SERVER['REMOTE_ADDR'] . "</td></tr><tr><td>Time</td><td></td><td>" . strftime("%Y-%m-%d %H:%M:%S",time()) . "</td></tr>"
    . "</table>";
  $system = drupal_mail_system("openlayers", "locator");
  $message['result'] = $system->mail($message);
  //print_r($component);
  //die;
  return $value;*/
//}


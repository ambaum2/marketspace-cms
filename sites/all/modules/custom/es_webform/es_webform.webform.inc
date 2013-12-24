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
  //$magento = new magento();
  //$product = new product();
  $mspaceApi = new mspaceApi;
  $webform = node_load($component['nid']);
  if(isset($webform->field_market_space_settings['und'][0]['attribute_set_id'])) {
    $attribute_set_id = $webform->field_market_space_settings['und'][0]['attribute_set_id'];
    //CHANGE
    //$attributes = $magento->get_all_attributes($attribute_set_id, $magento->decryptText(variable_get('session_id')));
    
    $form['extra']['magento_attribute_code'] = array(
      '#type' => 'select',
      '#title' => t('Magento Attribute'),
      '#options' => $mspaceApi->apiTokenCall('product/v1/attribute/set/options/id/' . $attribute_set_id),//$product->create_attribute_select_options($attributes),
      '#default_value' => isset($component['extra']['magento_attribute_code']) ? $component['extra']['magento_attribute_code'] : '',
      '#required' => TRUE,
      /*'#ajax' => array(
        'callback' => 'get_attribute_info',
        'method' => 'replace',
        'wrapper' => 'edit-extra-attribute-info'//apparenly this has to be the id of the destination element
      )*/
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
  //$magento = new magento();
  //watch('')
  //$attribute = $magento->get_attribute_information($magento->decryptText(variable_get('session_id')), $form['extra']['magento_attribute_code']['#value']);
  //$attribute = $mspaceApi->apiTokenCall('product/v1/attribute/type/data/code/' . $form['extra']['magento_attribute_code']['#value']);
  //($attribute['is_required'] == 1 ? $required = 'Yes' : $required = 'No');
  //$attribute_code_name = $attribute['code'];
  /*$attribute_info ='<div id="edit-extra-attribute-info"><span>Frontend Name: ' . $attribute['frontend_label'][0]['label'] 
    . '</span><br /><span>Form Type: ' . $attribute['frontend_input'] . '</span><br /><span>Required: ' 
    . $required . '</div>'; //$attribute->frontend_label[0]->label*/
  $commands = array();
  //$commands[] = ajax_command_before('edit-extra-attribute-info', $attribute_info);
  $commands[] = ajax_command_replace("#attribute_code_name", '<div id="attribute_code_name"><input type="hidden" value="' . $attribute['code'] . '" name="extra[magento_attribute_code_name]"></div>');
  //$commands[] = ajax_command_replace("#edit-extra-attribute-info", $attribute_info);
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Implements _webform_render_[component]().
 */
function _webform_render_es_webform($component, $value = NULL, $filter = TRUE, $form_state) {
  global $user;
  $magento_attribute = new magento_product_attributes();
  $product = new product_edit;//$product = new product();
  $attribute = $magento_attribute->get_stored_magento_attribute_data_by_code($component['extra']['magento_attribute_code']);
	if($attribute) {
  	$form = $product->create_attribute_input($attribute, $form_state, $component);
	} else {
		$form = "";
		drupal_set_message("could not load attribute please contact support ambaum2@gmail.com this is probably a one time thing", 'notice');
		watchdog('Could not get attribute', ' component data:<pre>%data</pre>', array('%data'=>print_r($component,true)), WATCHDOG_NOTICE);
	}
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
	$magento_product_attributes = new magento_product_attributes();
  if($submission->is_draft == 0 && isset($node->field_market_space_settings['und'][0])) { //if not a draft and a marketspace form
	  $default_magento_attributes = $magento_product_attributes->getDefaultAttributes();
	  $default_magento_stock_attributes = $magento_product_attributes->getDefaultStockAttributes();
		$product_add_data = $magento_product_attributes->createProductAddData($submission, $node); //returns an array with ['additional_attributes'] and ['attributes'] ['media_data]
  	$magento = new magento();
    $magento_media = new magento_media();
    $magento_product = new magento_product;
    //set defaults for required or ones that we want set for sure
  	$product_add_data['attributes']['websites'] = array(1);
  	!isset($product_add_data['attributes']['status']) ? $product_add_data['attributes']['status'] = '1' : ''; //2 is disabled oddly enough
  	!isset($product_add_data['attributes']['url_key']) ? $product_add_data['attributes']['url_key'] = $product_add_data['attributes']['name'] : '';
  	$product_add_data['attributes']['url_path'] = $product_add_data['attributes']['name'];
  	!isset($product_add_data['attributes']['visibility']) ? $product_add_data['attributes']['visibility'] = '4' : '';
  	!isset($product_add_data['attributes']['price']) ? $product_add_data['attributes']['price'] = 0 : '';
    //this makes no sense to me why its single_data but it is 
		!isset($product_add_data['attributes']['single_data']['page_layout']) ? $product_add_data['attributes']['single_data']['page_layout'] = 'one_column' : '';
		//TODO: obviously these need to be dynamic 
    $product_add_data['additional_attributes']['single_data'][] = array('key' => 'product_type', 'value' => $node->field_market_space_settings['und'][0]['product_type']);//;
    $product_add_data['additional_attributes']['single_data'][] = array('key' => 'marketspace_owner', 'value' => $user->uid);
    $product_add_data['attributes']['additional_attributes'] = $product_add_data['additional_attributes'];
    //if sku isset then we have to set the sku variable  - don't know if variable needed 
    !isset($product_add_data['attributes']['sku']) ? $sku = substr(md5($product_add_data['attributes']['name']. time()),0,32) : $sku = $product_add_data['attributes']['sku'];
		watchdog('New Product Submission', ' added product data<pre>%data</pre>', array('%data'=>print_r($product_add_data['attributes'],true)), WATCHDOG_NOTICE);
    $product_id = $magento->create_product($magento->decryptText(variable_get('session_id')), $node->field_market_space_settings['und'][0]['type_code'],
    	$node->field_market_space_settings['und'][0]['attribute_set_id'], $sku, $product_add_data['attributes']);
    if($product_add_data['media_data']) {
      foreach($product_add_data['media_data'] as $key => $value) { //add each piece of media to magento
        $file_output = $magento_media->create_media($product_id, $value);
      }
    }
    if(isset($product_add_data['attributes']['inventory_data']['manage_stock'])) { //TODO you don't actually need to do this you can add it to the add array - but there are problems getting stock data (no good api function)
      $result = $magento_product->updateProductStockData($product_id, $product_add_data['attributes']['inventory_data']);
    }
  }
}

/*
 * hook webform_component_presave
 * mofdify component before submitted
 * check if magento component (es_webform) if
 * so check database if exists for this nid
 * and cid 
 * Must save attribute as an object to the 
 * database
 */
function es_webform_webform_component_presave(&$component) {
  if($component['type'] == "es_webform") { //if this is a magento component
    //check webform_es_attribute for the magento_attribute_code id
    if(isset($component['extra']['magento_attribute_code'])) {
    module_load_include("inc", "es_webform", "includes/magento");
    module_load_include("inc","es_webform", "includes/magento_product_attributes");
    //$magento = new magento();
    $magento_attributes = new magento_product_attributes();
		$mspaceApi = new mspaceApi;
    //$attribute = $magento->get_attribute_information($magento->decryptText(variable_get('session_id')), $component['extra']['magento_attribute_code']);
    $attribute = (object)$mspaceApi->apiTokenCall('product/v1/attribute/type/data/code/' . $component['extra']['magento_attribute_code']);
    if($attribute->attribute_code == 'tax_class_id') { //special fix for tax_class_id
    	array_unshift($attribute->options, (object)array('label'=>'None', 'value'=>0)); //magento has a none option in admin but it does not get pulled in with api
    }
	  if($magento_attributes->exists_magento_attribute($attribute->attribute_id)) {
      //update
      $magento_attributes->update_magento_attribute($attribute);
    } else {
      //insert
      $magento_attributes->insert_magento_attribute($attribute);
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


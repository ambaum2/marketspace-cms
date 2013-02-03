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
      'payment_currency_code' => 'XXX',
      'payment_description' => '',
      'payment_line_items' => array(),
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_es_webform($component) {
  module_load_include("inc", "es_webform", "includes/magento");
  module_load_include("inc", "es_webform", "includes/product");
  //$magento = magento::getInstance();
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
    $form['extra']['attribute_info'] = array(
      '#type' => 'item',
      '#title' => t('Attribute Info'),
    );
    $form['extra']['payment_description'] = array(
      '#type' => 'text_format',
      '#title' => t('Description'),
      '#default_value' => $component['extra']['payment_description'],
      '#format' => 'full_html'
    );

  } else {
     $form['extra']['warning'] = array(
      '#type' => 'item',
      '#title' => t('Magento Attribute'),
      '#markup' => "<p style='color:red;font-weight:900;'>You must set an attribute set id before you can select an attribute</p>",
    );
  }

  return $form;
}
/*
 * ['extra']['magento_attribute_code']['#value']
 */
function get_attribute_info($form, $form_state) {
  module_load_include("inc", "es_webform", "includes/magento");
  //$magento = magento::getInstance();
  $magento = new magento();
  $attribute = $magento->get_attribute_information($magento->decryptText(variable_get('session_id')), $form['extra']['magento_attribute_code']['#value']);
  ($attribute->is_required == 1 ? $required = 'Yes' : $required = 'No');
  $form['extra']['magento_attribute_code']['attribute_info'] = array('#type'=>'item',
    '#markup'=>'<p id="edit-extra-attribute-info"><span>Frontend Name: ' . $attribute->frontend_label[0]->label 
    . '</span><br /><span>Form Type: ' . $attribute->frontend_input . '</span><br /><span>Required: ' 
    . $required . '</p>'); //$attribute->frontend_label[0]->label
  return $form['extra']['magento_attribute_code']['attribute_info'];
}
/**
 * Implements _webform_render_[component]().
 */
function _webform_render_es_webform($component, $value = NULL, $filter = TRUE) {
  global $user;
  module_load_include("inc", "es_webform", "includes/magento");
  module_load_include("inc", "es_webform", "includes/product");
  module_load_include("inc", "es_webform", "includes/magento_product_attributes");
  $magento = new magento();
  $magento_attribute = new magento_product_attributes();
  $product = new product();
  $attribute = $magento_attribute->get_stored_magento_attribute_data($component['extra']['magento_attribute_code']);
  $attribute = unserialize($attribute->data);
  $form = $product->create_attribute_input($attribute);
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


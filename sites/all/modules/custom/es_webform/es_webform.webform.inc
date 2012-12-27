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
      'private' => TRUE,
    ),
  );
}

/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_es_webform($component) {
  module_load_include("inc", "es_webform", "includes/magento");
  module_load_include("inc", "es_webform", "includes/product");
  $magento = magento::getInstance();
  $product = new product();
  $attributes = $magento->get_all_attributes(14, $magento->decryptText(variable_get('session_id')));
  
  $form['extra']['magento_attribute_code'] = array(
    '#type' => 'select',
    '#title' => t('Magento Attribute'),
    '#options' => $product->create_attribute_select_options($attributes),
    '#default_value' => $component['extra']['magento_attribute_code'],
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'get_attribute_info',
      'method' => 'replace',
      'wrapper' => 'edit-extra-attribute-info'//apparenly this has to be the id of the destination element
    )
  );
  $form['extra']['payment_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $component['extra']['payment_description'],
    '#required' => TRUE,
  );
  $form['extra']['attribute_info'] = array(
    '#type' => 'item',
    '#title' => t('Attribute Info'),
    '#markup' => "<p>The Info</p>",
  );
  $form['extra']['payment_line_items'] = array(
    '#type' => 'payment_line_item',
    '#title' => t('Line items'),
    '#cardinality' => 0,
    '#default_value' => $component['extra']['payment_line_items'],
    '#required' => TRUE,
  );

  return $form;
}
function get_attribute_info($form, $form_state) {
  $output = "<pre><span style='font-size:16px;'>" . date("g:i:s a",time()) . "</span><br />" . print_r($form,true) . "</pre>";
  return $form['extra']['attribute_info'] = array('#markup'=>'<p id="edit-extra-attribute-info">' . $output . '</p>'); 
}
/**
 * Implements _webform_render_[component]().
 */
function _webform_render_es_webform($component, $value = NULL, $filter = TRUE) {
  global $user;
  module_load_include("inc", "es_webform", "includes/magento");
  module_load_include("inc", "es_webform", "includes/product");
  $magento = magento::getInstance();
  $product = new product();
  echo $product->_convert_attribute_type("text");
  echo variable_get('session_id');
  echo "<br />unencrypted: " . $magento->decryptText(variable_get('session_id'));
  echo "<pre>" . print_r($component,true) . "</pre>";
  echo "<pre>" . print_r($magento->get_attribute_information($magento->decryptText(variable_get('session_id')), $component['extra']['magento_attribute_code']),true) . "</pre>";
  $form = array(
    '#type' => 'textfield',
    '#title' => $component['name'],
    '#default_value' => $component['extra']['magento_attribute_code'],
    '#required' => $component['mandatory'],
    //'#payment_line_items' => $component['extra']['payment_line_items'],
    //'#payment_currency_code' => $component['extra']['payment_currency_code'],
    //'#payment_add_page_path' => 'es_webform/pay/' . $component['nid'] . '/' . $component['cid'],
    //'#payment_load_callback' => 'es_webform_load',
    //'#payment_load_arguments' => array($component['cid'], $user->uid),
  );

  return $form;
}

/**
 * Implements _webform_submit_[component]().
 */
function _webform_submit_es_webform($component, $value) {
  //es_webform_delete_by_pid($value);

  return $value;
}
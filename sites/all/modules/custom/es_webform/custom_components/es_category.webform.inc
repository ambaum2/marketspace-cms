<?php

/**
 * Contains Webform component callbacks.
 */

/**
 * Implements _webform_defaults_[component]().
 */
function _webform_defaults_es_category() {
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
function _webform_edit_es_category($component) {
  module_load_include("inc", "es_webform", "includes/magento");
  module_load_include("inc", "es_webform", "includes/product");
  module_load_include("inc", "es_webform", "includes/magento_product_category");
  //$magento = magento::getInstance();
  $magento = new magento();
  $product = new product();
  $category = new magento_product_category();
  $webform = node_load($component['nid']);
  if(isset($webform->field_attribute_set_id['und'][0]['value'])) {
    //$attribute_set_id = $webform->field_attribute_set_id['und'][0]['value'];
    $category->get_category_tree($magento->decryptText(variable_get('session_id')));
    $all_categories = $category->get_categories();
    //$category_selections = $category->create_category_select($all_categories);
    //$category->display_category_tree($all_categories);
    //echo "<pre>" . print_r($categories,true);
    //die;
    $form['extra']['magento_categories'] = array(
      '#type' => 'fieldset',
      '#title' => 'Categories',
    );
    
    $category->create_category_checkboxes($all_categories,$form['extra']['magento_categories']);
  } else {
     $form['extra']['warning'] = array(
      '#type' => 'item',
      '#title' => t('Magento Attribute'),
      '#markup' => "<p style='color:red;font-weight:900;'>You must set an attribute set id before you can choose categories</p>",
    );
  }

  return $form;
}

/**
 * Implements _webform_render_[component]().
 */
function _webform_render_es_category($component, $value = NULL, $filter = TRUE) {
  global $user;
  module_load_include("inc", "es_webform", "includes/magento");
  module_load_include("inc", "es_webform", "includes/product");
  $magento = new magento();
  $product = new product();
  $attribute = get_stored_magento_attribute_data($component['extra']['magento_attribute_code']);
  $attribute = unserialize($attribute->data);
  $form = $product->create_attribute_input($attribute);
  return $form;
}

function _webform_display_es_category($component, $value, $format = 'html') {

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
function es_category_webform_component_presave(&$component) {
  if($component['type'] == "es_category") { //if this is a magento component
    //check webform_es_attribute for the magento_attribute_code id
    if(isset($component['extra']['magento_attribute_code'])) {
    module_load_include("inc", "es_webform", "includes/magento");
      module_load_include("inc", "es_webform", "includes/magentoproduct_attributes");
    $magento = new magento();
    $attribute = $magento->get_attribute_information($magento->decryptText(variable_get('session_id')), $component['extra']['magento_attribute_code']);
      if(exists_magento_attribute($component['extra']['magento_attribute_code'])) {
        //update
        update_magento_attribute($component,serialize($attribute));
      } else {
        //insert
        insert_magento_attribute($component,serialize($attribute));
      }
    }
  }
}


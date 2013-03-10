<?php

/**
 * Contains Webform component callbacks.
 */

/**
 * Implements _webform_defaults_[component]().
 */
function _webform_defaults_es_magento_custom_options() {
  return array(
    'extra' => array(
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_es_magento_custom_options($component) {
  $magento = new magento();
  $product = new product();
  $category = new magento_product_category();
  $webform = node_load($component['nid']);
  if(isset($webform->field_attribute_set_id['und'][0]['value'])) {
    $category->get_category_tree($magento->decryptText(variable_get('session_id')));
    $all_categories = $category->get_categories();
    $form['extra']['magento_attribute_code_name'] = array(
      '#type' => 'hidden',
      '#value' => "categories",
    );
    $form['extra']['magento_categories'] = array(
      '#type' => 'fieldset',
      '#title' => 'Categories',
    );
    $category->create_category_checkboxes($all_categories,$form['extra']['magento_categories'],$component['extra']);
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
function _webform_render_es_magento_custom_options($component, $value = NULL, $filter = TRUE, $form_state) {
  global $user;
  $magento = new magento();
  $product = new product();
  $category = new magento_product_category();
  $path2 = "";
  //if(in_array('authenticated_user',$user->roles)) {
    if(isset($form_state['values']['submitted'])) {
      //echo "<pre>" . print_r($form_state['values']['submitted'],true) . "</pre>";
      $selected_values = $form_state['values']['submitted'][$component['cid']]['categories'];
    } else {
      $selected_values = array();
    }
    $the_path = array_searchRecursive( "Array", $component, $strict=false, $path=array(), $path2);
    $checked_categories = explode(",",$path2);
    $categories = $category->get_categories($checked_categories,$excluded_categories = array(2));
    
    //render category checkboxes
    isset($_POST['form_build_id']) ? $form_build_id = $_POST['form_build_id'] : $form_build_id = "";
    //echo $form_build_id;
    $category->create_category_checkboxes($categories, $category_select_element, $component, true, $selected_values);
    return $category_select_element;
  //}
}

function _webform_display_es_magento_custom_options($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title', 'field_prefix', 'field_suffix'),
  );
}



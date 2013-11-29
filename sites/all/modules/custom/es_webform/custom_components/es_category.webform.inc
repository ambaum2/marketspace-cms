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
function _webform_edit_es_category($component) {
  $magento = new magento();
  $product = new product();
  $category = new magento_product_category();
  $webform = node_load($component['nid']);
  if(isset($webform->field_market_space_settings['und'][0]['attribute_set_id'])) {
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
     * Searches haystack for needle and 
     * returns an array of the key path if 
     * it is found in the (multidimensional) 
     * array, FALSE otherwise.
     *
     * @mixed array_searchRecursive ( mixed needle, 
     * array haystack [, bool strict[, array path]] )
     * $path2 builds a string of selected values
     */
 
function array_searchRecursive( $needle, $haystack, $strict=false, $path=array(), &$path2="" )
{
    if( !is_array($haystack) ) {
        return false;
    }
    foreach( $haystack as $key => $val ) {          
        if(strstr($key, "i") && $val == 1) {
          strlen($path2) > 0 ? $path2 .= "," . str_replace("i", "", $key) 
            : $path2 .= str_replace("i", "", $key);         
        }
        if( is_array($val) && $subPath = array_searchRecursive($needle, $val, $strict, $path, $path2) ) {
            $path = array_merge($path, array($key), $subPath);
            return $path;
        } elseif( (!$strict && $val == $needle) || ($strict && $val === $needle) ) {
            $path[] = $key;
            return $path;
        }
    }
    return false;
}

/**
 * Implements _webform_render_[component]().j
 * $component has information about component
 * $form state is a hack of webform to get it to show up
 */
function _webform_render_es_category($component, $value = NULL, $filter = TRUE, $form_state) {
  global $user;
  $magento = new magento();
  $product = new product();
  $category = new magento_product_category();
  $path2 = "";
  //if(in_array('authenticated_user',$user->roles)) {
    //get the selected categories from admin 
    if(isset($form_state['values']['submitted'][$component['cid']]['categories'])) {
      $selected_values = $form_state['values']['submitted'][$component['cid']]['categories'];
    } else {
      $selected_values = array(); //set to empty if none set
    }
    $the_path = array_searchRecursive( "Array", $component, $strict=false, $path=array(), $path2);
    $checked_categories = explode(",",$path2);
    $categories = $category->get_categories($checked_categories,$excluded_categories = array(2));
    //render category checkboxes
    isset($_POST['form_build_id']) ? $form_build_id = $_POST['form_build_id'] : $form_build_id = "";
    $category->create_category_checkboxes($categories, $category_select_element, $component, true, $selected_values);
    return $category_select_element;
  //}
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
//function es_category_webform_component_presave(&$component) {
  //if($component['type'] == "es_category") { //if this is a magento component
    //check webform_es_attribute for the magento_attribute_code id
    /*echo "<pre>" . print_r($component,true) . "</pre>";
    die;
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
    }*/
  //}
//}


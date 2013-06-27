<?php
/**
 * Implements _webform_defaults_[component]().
 */
function _webform_defaults_dwg_stock() {
  return array(
    'extra' => array(
      //'inventory_attribute' => 'XXX',
      //'payment_description' => '',
      //'payment_line_items' => array(),
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_dwg_stock($component) {
  $webform = node_load($component['nid']);
  //dpm($webform);
  if(isset($webform->field_market_space_setup['und'][0]['attribute_set_id'])) {
    $attribute_set_id = $webform->field_market_space_setup['und'][0]['attribute_set_id'];
    //$attributes = $magento->get_all_attributes($attribute_set_id, $magento->decryptText(variable_get('session_id')));
    
    $form['extra']['inventory_attribute'] = array(
      '#type' => 'select',
      '#title' => t('Magento Stock Attributes'),
      '#options' => array('qty' => 'Quantity','is_in_stock' => 'In Stock?'),
      '#default_value' => isset($component['extra']['inventory_attribute']) ? $component['extra']['inventory_attribute'] : '',
      '#required' => TRUE,
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


/**
 * Implements _webform_render_[component]().
 */
function _webform_render_dwg_stock($component, $value = NULL, $filter = TRUE, $form_state) {
  global $user;
  $form = array(
    '#type' => 'textfield',
    '#title' => $component['name'],
    '#required' => ($component['mandatory']) ? TRUE : FALSE,
    '#description' => isset($component['extra']['description']) ? $component['extra']['description'] : '',
  );
  return $form;
}

function _webform_display_dwg_stock($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title', 'field_prefix', 'field_suffix'),
  );
}
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
  $webform = node_load($component['nid']);
  if(isset($webform->field_attribute_set_id['und'][0]['value'])) {
  	$custom_ops = new magento_custom_options();
		$product_custom_ops = new product_custom_options();
		$options = $custom_ops->getAllCustomOptionsTypes();
    $form['extra']['magento_attribute_code_name'] = array(
      '#type' => 'hidden',
      '#value' => "custom_options",
    );
		
    $form['extra']['magento_custom_options'] = array(
			'#type' => 'fieldset',
			'#title' => t('Custom Options'),
		);//$product_custom_ops->renderOptionInfoForm($options, 'p');//$product_custom_ops->renderOptionTypeSelect($options, "p", "title", "description");
		$form['extra']['magento_custom_options']['addOption'] = array(
			'#type' => 'image_button',
			'#src' => drupal_get_path('module', 'es_magento_custom_options') . '/images/add_btn_icon.gif',
			'#ajax' => array(
        'callback' => 'add_custom_option',
        'method' => 'append',
        'wrapper' => 'edit-extra-magento-custom-options'//apparenly this has to be the id of the destination element
      )
		);
  } else {
     $form['extra']['warning'] = array(
      '#type' => 'item',
      '#title' => t('Magento Attribute'),
      '#markup' => "<p style='color:red;font-weight:900;'>You must set an attribute set id before you can choose custom options</p>",
    );
  }
  return $form;
}

/**
 * Implements _webform_render_[component]().
 */
function _webform_render_es_magento_custom_options($component, $value = NULL, $filter = TRUE, $form_state) {
  global $user;

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



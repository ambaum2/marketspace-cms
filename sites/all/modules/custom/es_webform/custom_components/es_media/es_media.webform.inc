<?php

/**
 * Contains Webform component callbacks.
 */

/**
 * Implements _webform_defaults_[component]().
 */
function _webform_defaults_es_media() {
  return array(
    'extra' => array(
      'private' => FALSE,
    ),
  );
}
/*
 * hook component_presave
 */
function es_media_webform_component_presave(&$component) {

}
/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_es_media($component, $form_state) {
  // Use the #managed_file FAPI element to upload an image file.
   $form['extra']['magento_attribute_code_name'] = array(
      '#type' => 'hidden',
      '#value' => "product_image",
    ); 
  // Because we have many fields with the same values, we have to set
  // #tree to be able to access them.
  return $form;
}


/**
 * Implements _webform_render_[component]().
 * 
 * for reference 
 */
function _webform_render_es_media($component, $value = NULL, $filter = TRUE, $form_state) {
  global $user;
  $form['product-image'] = array(
    '#title' => t('Image'),
    '#type' => 'managed_file',
    '#description' => t('Upload an image: '),
    '#process' => array("marketspace_image_upload"),
    '#upload_location' => 'public://u' . $user->uid,
  );

  return $form;
}
function marketspace_image_upload($element, &$form_state, $form) {
  $element = file_managed_file_process($element, $form_state, $form);
  return $element;
}
/*
 * _webform_display_[component]().
 *
 */
function _webform_display_es_media($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title', 'field_prefix', 'field_suffix'),
  );
}



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
  $form['image_manager'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="image-container">',
    '#suffix' => '</div>',
  );
  $form['image_manager']['product-image-main'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="image-container-child">',
    '#suffix' => '</div>',
  );
  _es_media_render_image_form_fields($form['image_manager']['product-image-main']);
  
  $form['image_manager']['product-image-thumbnail'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="image-container-child">',
    '#suffix' => '</div>',
  );  
  _es_media_render_image_form_fields($form['image_manager']['product-image-thumbnail']);
  /*$form['image_manager']['product-image-main'] = array(
    '#title' => t('Main Image'),
    '#type' => 'managed_file',
    '#description' => t('Upload an image: '),
    '#process' => array("marketspace_image_upload"),
    '#upload_location' => 'public://u' . $user->uid,
  );
  $form['image_manager']['product-image-thumbnail'] = array(
    '#title' => t('Thumbnail Image'),
    '#type' => 'managed_file',
    '#description' => t('Upload an image: '),
    '#process' => array("marketspace_image_upload"),
    '#upload_location' => 'public://u' . $user->uid,
  );*/
  return $form;
}

function _es_media_render_image_form_fields(&$component) {
  global $user;
  $component['image_fid'] = array(
    '#title' => t('Image Upload'),
    '#type' => 'managed_file',
    '#description' => t('Upload an image: '),
    '#process' => array("marketspace_image_upload"),
    '#upload_location' => 'public://u' . $user->uid,
  );
  $component['image_browser'] = array(
    '#type' => 'textfield',
    '#title' => t('Find Your Images'),
    '#autocomplete_path' => 'es_media/user/images',
    '#suffix' => '<div class="ms-autocomplete-results"></div>',
    '#attributes' => array('class' => array('ms-image-browser-autocomplete')),
  );
  $component['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Label'),
    '#description' => t('Write a label to make this field easily readable by search engines - do not use underscores'),
  );
  $component['position'] = array(
    '#type' => 'textfield',
    '#title' => t('Position'), 
  );
  $component['type'] = array(
    '#type' => 'select',
    '#title' => t(''),
    '#options' => array('main', 'small_image', 'thumbnail'),//$product->create_attribute_select_options($attributes),
    //'#default_value' => isset($component['extra']['magento_attribute_code']) ? $component['extra']['magento_attribute_code'] : '',
    '#required' => TRUE,
  );
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



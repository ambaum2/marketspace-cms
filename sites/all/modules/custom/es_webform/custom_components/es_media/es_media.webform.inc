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
function _webform_render_es_media($component, $value, $filter = TRUE, $form_state) {
  global $user;
  $form['product-image-add'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="image-container">',
    '#suffix' => '</div>',
  );
  $form['product-image-add']['product-image'] = array(
    '#title' => t('Upload new images to use them below'),
    '#type' => 'managed_file',
    '#description' => t('Upload images you want to use. Then search for them '),
    '#process' => array("marketspace_image_upload"),
    '#upload_location' => 'public://u' . $user->uid,
  );
  $form['image_manager'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="image-container">',
    '#suffix' => '</div>',
  );
	$image_components = _es_media_get_image_components();
	watchdog('es media', '<pre>%component</pre>', array('%component' => print_r($value, true)), WATCHDOG_NOTICE);
  foreach($image_components as $key => $image) {
	  $form['image_manager'][$key] = array(
	    '#type' => 'fieldset',
	    '#prefix' => '<div id="image-container-child">',
	    '#suffix' => '</div>',
	  );
		_es_media_render_image_form_fields($form['image_manager'][$key], $image);
  }
  return $form;
}

function _es_media_render_image_form_fields(&$component, $options) {
  global $user;
	$image_span = "";
	
	if(isset($component['image_fid'])) {
		$file = file_load($component['image_fid']);
		$image_url = file_create_url($file->uri);
		$image_name = $file->filename;
		//$image_span = '<span class="ms-image-browser-selection-info"><img height="50" width="50" src="">
		//	!image_name<input type="button" value="Remove" class="ms-image-browser-selection-info-button"></span>', array('!image_url' => $image_url, '!image_name' => $image_name));
	}
  $component['image_fid'] = array(
    '#type' => 'textfield',
    '#title' => t('<h2>%type</h2>', array('%type'=>$options['label'])),
    '#description' => t('start typing the name of your image to see it. Then click to choose it.'),
    '#autocomplete_path' => 'es_media/user/images',
    '#prefix' => t('<div class="ms-autocomplete-image-manager">!image_span', array('!image_span' => $image_span)),
    '#suffix' => '</div>',
    '#attributes' => array('class' => array('ms-image-browser-autocomplete')),
    '#default_value' => isset($component['image_fid']) ? $component['image_fid'] : '',
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
    '#prefix' => "<div style='display:none;'>",
    '#suffix' => "</div>",
    '#options' => array('main' => 'main', 'small_image' => 'small_image', 'thumbnail' => 'thumbnail'),//$product->create_attribute_select_options($attributes),
    '#default_value' => $options['type'],//isset($component['extra']['magento_attribute_code']) ? $component['extra']['magento_attribute_code'] : '',
    '#required' => TRUE,
  );
}

function _es_media_get_image_components() {
	$image_components = array(
		'product-image-main' => array(
			'label' => "Main Image",
			'type' => 'main'
		),
		'product-image-thumbnail' => array(
			'label' => "Thumbnail Image",
			'type' => 'thumbnail'
		),
	);
	return $image_components;
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



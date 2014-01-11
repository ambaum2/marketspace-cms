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
    '#type' => 'markup',
    '#prefix' => '<div id="image-container"><button class="btn btn-primary btn-lg" data-toggle="modal" data-target="#myModal">Image Manager</button>
    	<div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true"><div style="width:850px;" class="modal-dialog">
    	<div class="modal-content"><div class="modal-header"><button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button></div>
    	<iframe src="/?q=imce" frameborder="0" style="border: 1px solid #eee; width: 99%; height: 520px" class="imce-frame"></iframe>
    	</div></div></div>',
    '#suffix' => '</div>',
  );
  /*$form['product-image-add']['product-image'] = array(
    '#title' => t('Upload new images to use them below'),
    '#type' => 'managed_file',
    '#description' => t('Upload images you want to use. Then search for them '),
    '#process' => array("marketspace_image_upload"),
    '#upload_location' => 'public://u' . $user->uid,
  );*/
  $form['image_manager'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="image-container" class="row">',
    '#suffix' => '</div>',
  );
	$image_components = _es_media_get_image_components();
	//watchdog('es media', '<pre>%component</pre>', array('%component' => print_r($value, true)), WATCHDOG_NOTICE);
  foreach($image_components as $key => $image) {
	  $form['image_manager'][$key] = array(
	    '#type' => 'fieldset',
	    '#attributes' => array('class' => array('col-xs-6 col-sm-6 col-md-4')),
	    '#prefix' => '<div id="image-container-child">',
	    '#suffix' => '</div>',
	  );
		_es_media_render_image_form_fields($form['image_manager'][$key], $image);
  }
  return $form;
}

function _es_media_render_image_form_fields(&$component, $options) {
  global $user;

  $component['image_fid'] = array(
    '#type' => 'textfield',
    '#title' => t('<h2>%type</h2>', array('%type'=>$options['label'])),
    '#description' => t('start typing the name of your image to see it. Then click to choose it.'),
    '#autocomplete_path' => 'es_media/user/images',
    '#prefix' => '<div class="ms-autocomplete-image-manager">',
    '#suffix' => '</div>',
    '#required' => $options['required'],
    '#attributes' => array('class' => array('ms-image-browser-autocomplete')),
    '#default_value' => isset($component['image_fid']) ? $component['image_fid'] : '',
  );
  $component['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Label'),
    '#description' => t('Write a label to make this field easily readable by search engines - do not use underscores'),
  );
  $component['position'] = array(
    '#type' => 'hidden',
    '#value' => $options['position'],
  );
  $component['type'] = array(
    '#type' => 'select',
    '#title' => t(''),
    '#prefix' => "<div style='display:none;'>",
    '#suffix' => "</div>",
    '#options' => array('none' => '', 'image' => 'image', 'small_image' => 'small_image', 'thumbnail' => 'thumbnail'),//$product->create_attribute_select_options($attributes),
    '#default_value' => $options['type'],//isset($component['extra']['magento_attribute_code']) ? $component['extra']['magento_attribute_code'] : '',
    '#required' => TRUE,
  );
}

function _es_media_get_image_components() {
	$image_components = array(
		'product-image-main' => array(
			'label' => "Upper Left Image",
			'type' => 'image',
			'position' => 1,
			'required' => true,
		),
		'product-image-thumbnail' => array(
			'label' => "Thumbnail Image",
			'type' => 'thumbnail',
			'position' => 2,
			'required' => true,
		),
    /*'product-image-small-image' => array(
      'label' => "Catalog Image",
      'type' => 'small_image',
      'position' => 3,
    ),*/
	);
	for($i=1; $i < 5; $i++) {
		$image_components['product-image-small-' . $i] = array(
			'label' => "Main " . $i,
			'type' => 'none',
			'position' => $i + 3,
			'required' => false,
		);
	}
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



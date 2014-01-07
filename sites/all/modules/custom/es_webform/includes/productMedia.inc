<?php

class productMedia extends mspaceApi {
	
	/**
	 * render an image drupal form api
	 * field
	 * @param component
	 * 	array 
	 * @param options
	 * 	array
	 */
	public function es_media_render_image_form_fields(&$component, $options, $componentInfo) {
	  global $user;
		$image_span = "";
		drupal_set_message(var_export($componentInfo,true), 'info');
		if(isset($componentInfo['image_fid'])) {
			$file = file_load($componentInfo['image_fid']);
			$image_url = file_create_url($file->uri);
			$image_name = $file->filename;
			$image_span = t('<span class="ms-image-browser-selection-info"><img height="50" width="50" src="!image_url">
				!image_name<input type="button" value="Remove" class="ms-image-browser-selection-info-button"></span>', array('!image_url' => $image_url, '!image_name' => $image_name));
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

}

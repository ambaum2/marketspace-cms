<?php

class productMedia extends mspaceApi {
    
  /**
   * create the porduct edit media manager
   * @param form
   *  drupal form array
   * @param images
   *  array of image objects from magento api call
   * @param options
   *  array of options: current_fieldset
   */
  public function createMediaImageContainer(&$form, $images, $options) {
    $form[$options['current_fieldset']]['image_manager'] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );
    //assume there are at least two images and add 6-count(images)
    for($i=count($images); $i <= 5; $i++) {
      $blank = new stdClass();
      $blank->url = "";
      $blank->position = $i + 2;
      $blank->file = "";
      $blank->label = "";
      
      $images[$i] = $blank;
    }
    foreach($images as $key=>$image) {
      if($key % 3 == 0) {
        $rowNumber = ($key / 3);
        $form[$options['current_fieldset']]['image_manager'][$rowNumber] = array(
          '#type' => 'container',
          '#tree' => TRUE,
          '#attributes' => array('class' => array('row')),
        );       
      }
      $settings = $this->getImageSettings($image, $key);
      $form[$options['current_fieldset']]['image_manager'][$rowNumber][$key] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('col-xs-6 col-sm-6 col-md-4')),
        '#prefix' => '<div id="image-container-child">',
        '#suffix' => '</div>',
        '#weight' => $settings['image_weight'],
      );
      
      $this->createMediaImageFormFields($form[$options['current_fieldset']]['image_manager'][$rowNumber][$key], $image, $settings);
    }
    //for($i = 0; $i < count($images))
  }
  
  
  /**
   * return the weight for this image
   * based upon its type
   * @param image
   *  image object
   * @param key
   *  int key of image position in images object
   * @return int
   *  return the weight
   */
  public function getMediaWeight($image, $key) {
    $imageType = $this->getImageType($image); 
    switch ($imageType) {
      case 'image':
        $weight = 1;
        break;
      case 'small_image':
        $weight = 2;
        break;
      default:
        $weight = $key + 1;
        break;
    }
    return $weight;
  }
  /**
   * get image settings from image object
   * @param $image
   * @return array 
   *  array of settings: image_type, image_input_label
   */
  public function getImageSettings($image, $key) {
    $settings = array();
    $settings['image_type'] = $this->getImageType($image);
    $settings['image_weight'] = $this->getMediaWeight($image, $key);
    $settings['image_input_label'] = $this->getImageInputTypeLabel($settings['image_type'], $key);
    
    return $settings;
  }
  
  /**
   * get image input label
   * @param type
   *  string the image type: main, thumbnail, small_image, none
   * @param key 
   *  image index
   * @return string
   *  label 
   */
  public function getImageInputTypeLabel($type, $key) {
    $label = "";
    switch ($type) {
      case 'image':
        $label = "Upper Left Image";
        break;
      case 'small_image':
        $label = "Catalog Image";
        break;
      case 'thumbnail':
        $label = "Thumbnail Image";
        break;
      default:
        $label = "Main " . ($key - 2);
        break;
    }
    return $label;
  }
  /**
   * get image type from image object
   * @param image
   *  image object
   * @return image type or none
   *  string of type
   */
  public function getImageType($image) {
    (isset($image->types[0])) ? $imageType = $image->types[0]  : $imageType = "none";
    return $imageType;
  }
	/**
	 * render an image drupal form api
	 * field
	 * @param component
	 * 	array. a drupal renderable array
	 * @param image
	 * 	object of magneto image see magento_mediaTest.php
   * @param values
   *  current form values from form_state['values']
   * @param settings
   *  image settings see $this->getImageSetings
	 */
	public function createMediaImageFormFields(&$component, $image, $settings, $values = null) {
	  global $user;
    $component['image_thumb'] = array(
      '#markup' => ($image->url != "" ? t('<h2>%type</h2><img src="!src" width="auto" height="120px" />', array('%type'=>$settings['image_input_label'], '!src' => $image->url)) 
        : t('<h2>%type</h2><div class="ms-image-placeholder-image"></div>', array('%type'=>$settings['image_input_label']))),
    );
	  $component['image_fid'] = array(
	    '#type' => 'textfield',
	    '#title' => t('', array('%type'=>$settings['image_input_label'], '!src' => $image->url)),
	    '#description' => t('start typing the name of the image you want to replace'),
	    '#autocomplete_path' => 'es_media/user/images',
	    '#prefix' => t('<div class="ms-autocomplete-image-manager">', array()),
	    '#suffix' => '</div>',
	    '#attributes' => array('class' => array('ms-image-browser-autocomplete')),
	    '#default_value' => isset($values['image_fid']) ? $values['image_fid'] : '',
	  );
	  $component['label'] = array(
	    '#type' => 'textfield',
	    '#title' => t('Image Label'),
	    '#description' => t('Write a label to make this field easily readable by search engines - do not use underscores'),
	    '#default_value' => isset($image->label) ? $image->label : '',
	  );
    $component['position'] = array(
      '#type' => 'hidden',
      '#value' => $image->position,
    );
    $component['type'] = array(
      '#type' => 'select',
      '#title' => t(''),
      '#prefix' => "<div style='display:none;'>",
      '#suffix' => "</div>",
      '#options' => array('none' => 'none', 'image' => 'image', 'small_image' => 'small_image', 'thumbnail' => 'thumbnail'),//$product->create_attribute_select_options($attributes),
      '#default_value' => $this->getImageType($image),//isset($component['extra']['magento_attribute_code']) ? $component['extra']['magento_attribute_code'] : '',
      '#required' => TRUE,
    );
    $component['current_image'] = array(
      '#type' => 'hidden',
      '#value' => $image->file,
    );
    $component['original_label'] = array(
      '#type' => 'hidden',
      '#value' => $image->label,
    );
	}

}

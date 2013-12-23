<?php

class magento_media extends magento {
  var $session_id;
  function __construct() {
    parent::__construct();
    $this->session_key = parent::decryptText(variable_get('session_id'));
  }
  
  /**
   * create new media for a product
   * @param product_id | integer 
   *  product id of product for this custom option in magento
   * @param data | array
   *  keyed array of media data
   * @return boolean 
   *  true if worked false otherwise
   */
  public function create_media($product_id, $data) {
    $proxy = new SoapClient(variable_get("magento_api_wsdl"));
    try {    
      $result = $proxy->catalogProductAttributeMediaCreate(
        $this->session_key,
        $product_id,
        $data
      );
      return $result;
    } catch (exception $e) {
    	drupal_set_message('data array error: please contact the admin', '<pre>' . print_r($data,true). "</pre>");
    	watchdog('magento media', 'image create failed. <pre>%e</pre> <pre>%data</pre>', array('%e'=>print_r($e,true), '%data'=>print_r($data,true)),WATCHDOG_ERROR);
      throw $e;
    }
  }

  /**
   * get all product media
   * @param product_id | integer 
   *  product id of product for this custom option in magento
   * @return xml string 
   *  string of xml data for media for product 
   */
   public function get_all_media($product_id) {
     $proxy = new SoapClient(variable_get("magento_api_wsdl"));
     try {    
        $result = $proxy->catalogProductAttributeMediaList($this->session_key, $product_id);
        return $result;
     } catch (exception $e) {
        throw $e;
     }
   }
   
  /**
   * update product media data
   * 
   * @param product_id | integer 
   *  product id of product for this custom option in magento
   * @param data | array
   *  keyed array of media data
   * @return boolean 
   *  true if worked false otherwise
   */
  public function update_media_by_product_id($product_id, $data) {
    $proxy = new SoapClient(variable_get("magento_api_wsdl"));
    try {    
       $result = $proxy->catalogProductAttributeMediaUpdate($this->session_key, $product_id, $data['file']['name'], $data);
       return $result;
    } catch (exception $e) {
       throw $e;
    }
  }
  
  /**
   * remove product media
   * @param product_id | integer 
   *  product id of product for this custom option in magento
   * @param file_name | string
   *  name of file
   * @return boolean 
   *  true if worked false otherwise
   */
  public function remove_media_by_product_id($product_id, $file_name) {
    $proxy = new SoapClient(variable_get("magento_api_wsdl"));
    try {    
       $result = $proxy->catalogProductAttributeMediaRemove($this->session_key, $product_id, $file_name);
       return $result;
    } catch (exception $e) {
       throw $e;
    }
  }

  /**
   * @param data | object
   *  image data
   * @return array
   * 
   * load data for magento image. Actually
   * get the contents of a file. This would 
   * not work for private files.
   * managed_file - is file from drupal or elsewhere
   */
  public function get_magento_media_data($data) { 
    $image = file_get_contents($data->url);
    $file_info = new finfo(FILEINFO_MIME_TYPE);
    $mime_type = $file_info->buffer($image);
    $file_content = base64_encode($image);
    $file = array(
      'content' => $file_content,
      'mime' => $mime_type,
      'name' => $data->file,
    );
    $magento_media_info = array('file'=>$file, 'label'=>$data->label, 'position'=>$data->position,
      'type' => array($data->type), 'exclude' => 0);
    return $magento_media_info;
  }
  /**
   * get data array for use in create edit delete
   * api calls which use get_mangeto_media_data
   * and that is the function to which the data
   * returned from this method should be passed
	 * @param input | 
   * @return array of media data
   * @todo i need to reaxamine this i don't fully understand it
	 * $input - form_state['input'] array
   */
  public function get_marketspace_media_data($input, $image_attributes, $new_media = false, $parent_container = 'image_manager', $container_name = 'image_container', $options_container = 'options') {
    $data = new stdClass();
    $index = (int)$image_attributes->index;
		//look at formstate input for image_manager
    $data->label = $input[$parent_container][$index][$container_name][$options_container]['label'];
    $data->position = $input[$parent_container][$index][$container_name][$options_container]['position'];
    if($new_media) { //first save the new file 
      global $user;
      $image_fid = $input[$parent_container][$index][$container_name]['image']['fid'];
      $this->process_managed_file($image_fid, "marketspace", "user", $user->uid); //we want to save this file although don't necessarily have to
      $file = file_load($image_fid);
      $data->file = $file->filename;
      $data->url = file_create_url($file->uri);  
    } else {
      $data->file = $input[$parent_container][$index][$container_name]['file'];
      $data->url = $input[$parent_container][$index][$container_name]['url'];   
    }
    return $data;
  }
  /**
   * get data array for images with only an fid
	 * this data is no good for media Create or 
	 * update on its own fro the api calls. You'll need
	 * to pass this into get_magento_media_data 
	 * to get workable data (content adn mime types)
	 * @param image_data
   * array of image options for creating in magento
   * 'image_fid' => int
   * 'label'=>string, 'position'=>int, 
   * 'image_type'=>string (main, thumbnail, small_image)
	 * @return array
   */
  public function get_marketspace_media_data_by_fid($image_data) {
    $data = new stdClass();
    global $user;
    $this->process_managed_file($image_data['image_fid'], "marketspace", "user", $user->uid); //we want to save this file although don't necessarily have to
    $file = file_load($image_data['image_fid']);
    $data->file = $file->filename;
    $data->url = file_create_url($file->uri);  
    $data->label = $image_data['label'];
    $data->position = $image_data['position'];
    $data->type = $image_data['type'];
    return $data;
  }
  /**
   * render product image form element
   * @param image_link | string
   *  url of image to create input
   * @return array
   *  renderable array for form element
   * TODO - this probably should be in its own class
   * along with process_managed_file
   */
  public function render_marketspace_image_input($image_link) {
    global $user;  
    $element['product-image'] = array(
      '#title' => t('Image'),
      '#type' => 'managed_file',
      '#description' => t('Upload an image: '),
      '#default_value' => !empty($image_link) ? $image_link : '',
      '#process' => array("marketspace_image_upload"),
      '#upload_location' => 'public://u' . $user->uid,
    );
    return $element;
  }
	/**
	 * save file if and make permanent or delete
	 * 
	 * Variables Needed:
	 * @param image_fid | integer
   *  for determining if file temporary (0) and delete
	 *  or permenant != 0 so save
	 * @param module | string
   *  marketspace or whatever module it belongs to
	 * @param type | string
   *  user possibly or any type of object that contains the reference
	 * @param id | integer
   *  id of user or entity that owns the file $global $user $user->uid
	 *  defaults to superuser
	 * @param style | string
   *  the image style default to $form_state['values']['image_example_style_name']
	 */
  // If fid is not 0 we have a valid file.
  public function process_managed_file($image_fid, $module = "marketspace", $type="user", $id = 1, $style = 'marketspace_profile_image') {
		if ($image_fid != 0) { //was $form_state['values']['image_example_image_fid']
		  // The new file's status is set to 0 or temporary and in order to ensure
		  // that the file is not removed after 6 hours we need to change it's status
		  // to 1. Save the ID of the uploaded image for later use.
		  $file = file_load($image_fid);
		  $file->status = FILE_STATUS_PERMANENT;
		  file_save($file);
		
		  // When a module is managing a file, it must manage the usage count.
		  // Here we increment the usage count with file_usage_add().
		  file_usage_add($file, $module, $type, $id);
		
		  // Save the fid of the file so that the module can reference it later.
		  variable_set('marketspace_image_fid', $file->fid);
		  //drupal_set_message(t('The image @image_name was uploaded and saved with an ID of @fid and will be displayed using the style @style.', array('@image_name' => $file->filename, '@fid' => $file->fid, '@style' => $style)));
		}
		// If the file was removed we need to remove the module's reference to the
		// removed file's fid, and remove the file.
		elseif ($image_fid == 0) {
		  // Retrieve the old file's id.
		  $fid = variable_get('marketspace_image_fid', FALSE);
		  $file = $fid ? file_load($fid) : FALSE;
		  if ($file) {
		    // When a module is managing a file, it must manage the usage count.
		    // Here we decrement the usage count with file_usage_delete().
		    file_usage_delete($file, $module, $type, $id);
		
		    // The file_delete() function takes a file object and checks to see if
		    // the file is being used by any other modules. If it is the delete
		    // operation is cancelled, otherwise the file is deleted.
		    file_delete($file);
		  }
		
		  // Either way the module needs to update it's reference since even if the
		  // file is in use by another module and not deleted we no longer want to
		  // use it.
		  variable_set('marketspace_image_fid', FALSE);
		  //drupal_set_message(t('The image @image_name was removed.', array('@image_name' => $file->filename)));
		}
		
		// Save the name of the image style choosen by the user.
		variable_set('marketspace_style_name', $style);
	}    
  /**
   * render_image_manager_element
   * output an image manager 
   * @param form | array
   *  form that contains image manager
   * @param key | integer
   *  key of element from iterator
   * @param image | object
   *  image object 
   * @param fieldset | string
   *  tab group the image manager is in
   * @param edit_form | boolean
   *  is this the edit or new form
   */
  public function render_image_manager_element(&$form, $key, $image, $fieldset, $edit_form = true) {
    $form[$fieldset]['image_manager'][$key]['image_container'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<div id="image-container-' . $key . '">',
      '#suffix' => '</div>',
    );
    if($edit_form) {
      $form[$fieldset]['image_manager'][$key]['image_container']['image'] = array(
        '#type' => 'item',
        '#markup' => '<img src="' . $image->url . '" style="width:75px; height:75px;float:left;margin:0px 5px 5px 0px;" />'
      );
    } else {
      global $user;
      $form[$fieldset]['image_manager'][$key]['image_container']['image'] = array(
        '#title' => t('Image'),
        '#type' => 'managed_file',
        '#description' => t('Upload an image: '),
        //'#default_value' => '',
        //'#process' => array("marketspace_image_upload"),
        '#upload_location' => 'public://marketspace/u' . $user->uid,
      );
    }
    $form[$fieldset]['image_manager'][$key]['image_container']['options'] = array(
      '#type' => 'fieldset',
      '#prefix' => '<div id="options-container' . $key . '">',
      '#suffix' => '</div><div style="clear:both;"></div>',
    );
    $form[$fieldset]['image_manager'][$key]['image_container']['options']['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#size' => 5,
      '#default_value' => isset($image->label) ? $image->label : '',
    );
    $form[$fieldset]['image_manager'][$key]['image_container']['options']['position'] = array(
      '#type' => 'textfield',
      '#title' => t('Position'),
      '#size' => 5,
      '#default_value' => isset($image->position) ? $image->position : '',
    );
    /*$form[$fieldset]['image_manager'][$key]['image_container']['options']['check_deleted'] = array(
      '#type' => 'checkbox',
      '#value' => t('Delete'),                 
    );*/
    if($edit_form) { //str_replace(array("/"), "\/" ,$image->file)
      $form[$fieldset]['image_manager'][$key]['image_container']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#name' => "delete_image_" . $key,
        '#prefix' => "<!--<image_edit><index>" . $key . "</index><file>" . $image->file . "</file></image_edit>-->",
        '#submit' => array('edit_marketspace_image_callback'),
      );
      $form[$fieldset]['image_manager'][$key]['image_container']['edit'] = array(
        '#type' => 'submit',
        '#value' => t('Edit'),
        '#name' => "edit_image_" . $key,
        '#prefix' => "<!--<image_edit><index>" . $key . "</index></image_edit>-->",
        '#suffix' => "",
        //'#submit' => array('edit_marketspace_image_callback'),
        '#ajax' => array(
          'callback' => 'edit_marketspace_image_callback',
          //'method' => 'replace',
          //'wrapper' => 'image-container-' . $key,
        ),
      );
      $form[$fieldset]['image_manager'][$key]['image_container']['file'] = array('#type' => 'hidden', '#value' => $image->file);
      $form[$fieldset]['image_manager'][$key]['image_container']['url'] = array('#type' => 'hidden', '#value' => $image->url);
    } else {
      $form[$fieldset]['image_manager'][$key]['image_container']['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add'),
        '#name' => "add_image_" . $key,
        '#prefix' => "<!--<image_edit><index>" . $key . "</index></image_edit>-->",
        '#submit' => array('edit_marketspace_image_callback'),
        //'#ajax' => array(
          //'callback' => 'edit_marketspace_image_callback',
          //'method' => 'replace',
          //'wrapper' => 'image-container-' . $key,
        //),
      );      
    }

  }
}
  
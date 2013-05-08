<?php

class magento_media extends magento {

  /*
   * create new media for a product
   */
   public function create_media($session_id, $product_id, $product_data, $file_path, $sku, $image_types = array('thumbnail','image'), $host = magento::primary_host) {
      $proxy = new SoapClient($host);
      $image = file_get_contents($file_path);
      $file_info = new finfo(FILEINFO_MIME_TYPE);
      $mime_type = $file_info->buffer($image);
      $file_content = base64_encode($image);
      $file = array(
      'content' => $file_content,
      'mime' => $mime_type,
      //'name' => $product_data['name'] . $sku,
      );
     try {    
        $result = $proxy->catalogProductAttributeMediaCreate(
          $session_id,
          $product_id,
          array('file' => $file, 'label' => 'a label', 'position' => '10', 'types' => $image_types, 'exclude' => 0)
        );
        return $result;
     } catch (exception $e) {
        throw $e;
     }
  }

	/*
	 * save file if and make permanent or delete
	 * 
	 * Variables Needed:
	 * $image_fid - for determining if file temporary (0) and delete
	 *  or permenant != 0 so save
	 * $module = marketspace or whatever module it belongs to
	 * $type - user possibly or any type of object that contains the reference
	 * $id - id of user or entity that owns the file $global $user $user->uid
	 * defaults to superuser
	 * $style - the image style default to $form_state['values']['image_example_style_name']
	 */
  // If fid is not 0 we have a valid file.
  public function process_managed_file($image_fid, $module = "marketspace", $type="user", $id = 1, $style = null) {
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
		  drupal_set_message(t('The image @image_name was uploaded and saved with an ID of @fid and will be displayed using the style @style.', array('@image_name' => $file->filename, '@fid' => $file->fid, '@style' => $style)));
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
		  drupal_set_message(t('The image @image_name was removed.', array('@image_name' => $file->filename)));
		}
		
		// Save the name of the image style choosen by the user.
		variable_set('image_example_style_name', $style);
	}    
}
  
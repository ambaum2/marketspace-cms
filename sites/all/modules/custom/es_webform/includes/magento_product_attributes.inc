<?php

class magento_product_attributes extends MS_Api {
    
   /*
   * insert new data into webform_es_attribute
   * 
   */
   public function insert_magento_attribute($attribute_info) {
    $id = db_insert('webform_es_attribute')
      ->fields(array(
        'mage_id' => $attribute_info->attribute_id,
        'code' => $attribute_info->attribute_code,
        'modified' => time(),
        'data' => serialize($attribute_info),//print_r($attribute_info,true)
        
      ))
      ->execute();
   }
   
   /*
   * update  data for webform_es_attribute
   * @TODO - put db functions into own class as 
   */
   public function update_magento_attribute($attribute_info) {
    $id = db_update('webform_es_attribute')
      ->fields(array(
      	'code' => $attribute_info->attribute_code,
      	'modified' => time(),
        'data' => serialize($attribute_info),
      ))
      ->condition('mage_id', $attribute_info->attribute_id)
      ->execute();
   }
	/**
	 * @param attribute_code | string
	 * 	the magento attribute code
	 * @return boolean
	 * 	returns attribute id if found false 
	 * 	otherwise 
	 */
	public function get_magento_attribute_id_by_code($attribute_code) {
    $query = db_select('webform_es_attribute', 'wea')
      ->fields('wea',array('mage_id'))
      ->condition('code',$attribute_code);
    $result = $query->execute()->fetch();
    if($result) {
      return $result->mage_id;
    } else {
      return false;
    }
   }		
   /*
   * see if record exists into webform_es_attribute
   * @TODO - put db functions into own class as 
   */
   public function exists_magento_attribute($attribute_id) {
    $query = db_select('webform_es_attribute', 'wea')
      ->fields('wea',array('mage_id'))
      ->condition('mage_id',$attribute_id);
    $result = $query->execute()->fetch();
    if($result) {
      return true;
    } else {
      return false;
    }
   }
   /**
	  * @param attribute_id
	  * 	the id of the magento attribute
	  * @return object
	  * 	a keyed object
	  */
   public function get_stored_magento_attribute_data($attribute_id) {
    $query = db_select('webform_es_attribute', 'wea')
      ->fields('wea',array('data'))
      ->condition('mage_id',$attribute_id);
    $result = $query->execute()->fetch();
    if($result) {
      return unserialize($result->data);
    } else {
      return false;
    }
   }
   /**
	  * 
	  * @param attribute_code
	  * 	magento attribute code
	  * 
	  * @return array
	  * 	return array if found or false if not found
	  */
   public function get_stored_magento_attribute_data_by_code($attribute_code) {
    $query = db_select('webform_es_attribute', 'wea')
      ->fields('wea',array('data'))
      ->condition('code',$attribute_code);
    $result = $query->execute()->fetch();
    if($result) {
      return unserialize($result->data);
    } else {
      return false;
    }
   }

	/**
	 * go through a submission and seperate the additional from
	 * the regular attributes
	 * 
	 * @param submission | array
	 * 	a form submission array
	 * @param node | array
	 * 	the webform node 
	 * @return array
	 * returns an array with ['additional_attributes'] 
	 * and ['attributes'] ['media_data]
	 */
	public function createProductAddData($submission, $node) {
		$data = array();
		foreach($submission->data as $item=>$value) {
			if(isset($node->webform['components'][$item]['extra']['magento_attribute_code'])) {
				$attribute_code = $node->webform['components'][$item]['extra']['magento_attribute_code'];
			  $mage_id = $this->get_magento_attribute_id_by_code($attribute_code);
				if($attribute = $this->get_stored_magento_attribute_data($mage_id)) { //if it is a standard catalog attribute
		  		if($attribute->frontend_input == "textarea") { //check format to see if textarea or wysiwyg
		  		  if(in_array($attribute_code, $this->getDefaultAttributes())) {
		  		      $data['attributes'][$attribute_code] = $value['value']['text']['textarea']['value']; //used to be ['value']['text']['textarea']['value']
						} else {
					      $data['additional_attributes']['single_data'][] = array('key'=>$attribute_code, 'value'=>$value['value']['text']['textarea']['value']);
						}
					} else {
		  				if($attribute->frontend_input == "multiselect") {
		  				  $selections = array();
	              foreach($value['value'] as $select_key=>$select_value) {
	                $selections[] = $select_value;
	              } 
		  				  if(in_array($attribute_code,$this->getDefaultAttributes())) {
			  					$data['attributes'][$attribute_code] =	$selections;
	              } else {
	                $data['additional_attributes']['multi_data'][] = array('key'=>$attribute_code, 'value'=>$selections);
	              }
							} else {
	              if(in_array($attribute_code, $this->getDefaultAttributes())) {
								    $data['attributes'][$attribute_code] = $value['value'][0];
	              } else {
	                  $data['additional_attributes']['single_data'][] = array('key'=>$attribute_code, 'value' => $value['value'][0]);
	              }
		          }
					}
				} //if not then not valid attribute : all other attributes must be exlplicitely referenced
			} elseif(isset($node->webform['components'][$item]['extra']['inventory_attribute'])) {
			 $data['attributes']['inventory_data'][$node->webform['components'][$item]['extra']['inventory_attribute']] = $value['value'][0];
			 if(!isset($data['attributes']['inventory_data']['manage_stock'])) { // automatically set manage stock to true if any inventory attribute is used
			   $data['attributes']['inventory_data']['manage_stock'] = 1;
			   }       
			} elseif(isset($value['value']['image_manager'])) { //probably want to handle all other types media, categories, custom attributes etc with $node->webform['components'][$item]['extra']['magento_attribute_code_name'] == "categories" or whatever type it is
          $data['media_data'] = $this->processMediaData($value['value']['image_manager']);
			} elseif(isset($value['value']['categories'])) {//this was old check think it fails isset($value['value']['magento_categories'])) {      
			  $categories = array();
			  foreach($value['value']['categories'] as $cat_key=>$cat_value) {
			    if($cat_value == 1) {
			      $categories[] = $cat_key;
			    }
			  }
			  $data['attributes']['categories'] = $categories;
			  }
			}
		return $data;
	}

  /**
   * process update/create product data
   * from a submission array
   * return data array for update/insert 
   * api call
   * 
   * note the returned array will have
   * an array called inventory_data 
   * where inventory attributes are stored
   * since they are handled seperatly from
   * attributes and additional_attributes
   * @param form_state | array @TODO form state is used to create input which is used 6 times this is redundant I already have 
	 * function like this in magento_product_attributes
	 *
   *  form state for submitted form
   * @param insert | boolean @TODO THIS IS NOT EVEN USED - Why do we have this 
   *  true if inserting false if updating
   * @return array
   *  returns product data as array for confirmation
   *
   * TODO Change submission to your array
   * TODO make a seperate function for getting
   * stock data array
   */
  public function getProductDataArray($input) {	
		$data = array();
		$node = node_load($input['product_edit_node_id']);
    foreach($node->webform["components"] as $item=>$value) {
      if(isset($node->webform["components"][$item]['extra']['magento_attribute_code'])) { //is this a magento component whether custom (custom options, categories, images etc) or built in
        $attribute_code = $node->webform['components'][$item]['extra']['magento_attribute_code'];
        $mage_id = $this->get_magento_attribute_id_by_code($attribute_code);
        if($attribute = $this->get_stored_magento_attribute_data($mage_id)) { //if it is a standard catalog attribute default or custom attribute
	        if($attribute->frontend_input == "textarea") { //could also check ->additional_fields[0]->is_wysiwyg_enabled
	          if(in_array($attribute_code, $this->getDefaultAttributes())) {//in_array($attribute->attribute_code,$default_magento_attributes)) {
	              $data[$attribute_code] = $input[$attribute_code]['value'];
	          } else { //custom attribute so put in additional attributes
	              $data['additional_attributes']['single_data'][] = array('key'=>$attribute_code, 'value'=>$input[$attribute_code]['value']);
	          }
	        } else { //for all other types 
		          if($attribute->frontend_input == "multiselect") {
		              $selections = array();
		              foreach($input[$attribute_code] as $select_key=>$select_value) {
		                $selections[] = $select_value;
		              } 
		              if(in_array($attribute_code, $this->getDefaultAttributes())) { //in_array($attribute_code,$default_magento_attributes)) {
		                $data[$attribute_code] = $selections;
		              } else {
		                $data['additional_attributes']['multi_data'][] = array('key'=>$attribute_code, 'value'=>$selections);
		              }
		          } else { //textfields, number, should be processed the same
		              if(in_array($attribute_code, $this->getDefaultAttributes())) {
		                  $data[$attribute_code] = $input[$attribute_code];
		              } else {
		                  $data['additional_attributes']['single_data'][] = array('key'=>$attribute_code, 'value' => $input[$attribute_code]);
		              }
		          }
	        }
	      //all other attributes must be exlplicitely referenced
      } //if not then a regular attribute could be category or inventroy
    } elseif(isset($node->webform["components"][$item]['extra']['inventory_attribute'])) {
    	 $data['attributes']['inventory_data'][$node->webform["components"][$item]['extra']['inventory_attribute']] = $input[$node->webform["components"][$item]['extra']['inventory_attribute']];
       if(!isset($data['attributes']['inventory_data']['manage_stock'])) { // automatically set manage stock to true if any inventory attribute is used
         $data['inventory_data']['manage_stock'] = 1;
       }
    } elseif(isset($node->webform["components"][$item]['extra']['magento_categories'])) {      
	      $categories = array();
	      foreach($input['categories'] as $cat_key=>$cat_value) {
	        if($cat_value == 1) {
	          $categories[] = $cat_key;
	        }
	      }
	      $data['categories'] = $categories;
  	} else {
  		
  	}
  }
	//$magento_product_data['additional_attributes'] = $magento_additional_attributes;
	//$data['additional_attributes'] = $data['additional_attributes'];
	return $data;
  }
  /**
   * process media data creates an array that works
   * with mediaCreate api call. This is for new images
   * @param mediaData
   *  an array of image arrays each one should 
   * be in this form:
   * array('image_fid' => 79, 'label' => "some label", 
   * 'position' => 100, 'type' =>string (main, thumbnail, small_image))
   * @return array
   *  array of media data for magento
   * 
   * @TODO NEED TO put access control function in here inside
   * foreach to check if the current user owns the image (users uid
   * is in the managed file table)
   */
  public function processMediaData($mediaData) {
    $magento_media = new magento_media;
    $data = array();
    foreach($mediaData as $key => $value) {
      if($value['image_fid'] > 0) {
       $data[] = $magento_media->get_magento_media_data($magento_media->get_marketspace_media_data_by_fid($value));
      }
    }
    return $data;
  }
	/**
	 * @return array
	 * returns an array of all dfault 
	 * attributes
	 */
	public function getDefaultAttributes() {
		return array('categories','websites','name','description','short_description','weight','status','url_key','url_path'
	    ,'visibility','category_ids','website_ids','has_options','gift_message_available','price','special_price','special_price','special_from_date'
	    ,'special_to_date','tax_class_id','meta_title','meta_keyword','meta_description','custom_design','custom_layout_update','options_container'
	    );
	}
	
	/**
	 * @return array
	 * returns array of all default stock 
	 * attributes 
	 */
	public function getDefaultStockAttributes() {
		return array('qty','is_in_stock','manage_stock','use_config_manage_stock','min_qty','use_config_manage_stock'
	    ,'min_qty','use_config_min_qty','min_sale_qty','use_config_min_sale_qty',' max_sale_qty',' use_config_max_sale_qty','');
	}
}

<?php

class magento_product_attributes extends mspaceApi {
    
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
			  //if it is a standard catalog attribute
				if($attribute = $this->get_stored_magento_attribute_data($mage_id)) {
		  		if(isset($value['value']['text']['textarea'])) { //check format to see if textarea or wysiwyg
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
								    $data['attributes'][$attribute_code] =	$value['value'][0];
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
			} elseif(isset($value['value']['product-image'])) { //probably want to handle all other types media, categories, custom attributes etc with $node->webform['components'][$item]['extra']['magento_attribute_code_name'] == "categories" or whatever type it is
			 $image_fid = $value['value']['product-image'];
			 $magento_media = new magento_media;
			 if($image_fid > 0) {
			  $data['media_data'] = $magento_media->get_magento_media_data($magento_media->get_marketspace_media_data_by_fid($image_fid, "none", $position = 100));
			 }
			 //set_variable('marketspace_image_new_fid_' . $node->nid, ''); //unset this because we don't want it to appear again
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

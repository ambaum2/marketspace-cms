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
   
   public function get_stored_magento_attribute_data($attribute_id) {
    $query = db_select('webform_es_attribute', 'wea')
      ->fields('wea',array('data'))
      ->condition('mage_id',$attribute_id);
    $result = $query->execute()->fetch();
    if($result) {
      return $result;
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
}

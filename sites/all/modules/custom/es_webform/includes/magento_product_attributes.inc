<?php

class magento_product_attributes extends magento {
    
   /*
   * insert new data into webform_es_attribute
   * @TODO - put db functions into own class as 
   */
   public function insert_magento_attribute($component, $attribute_info) {
    $id = db_insert('webform_es_attribute')
      ->fields(array(
        'mage_id' => $component['extra']['magento_attribute_code'],
        'data' => $attribute_info,//print_r($attribute_info,true)
      ))
      ->execute();
   }
   
   /*
   * update  data for webform_es_attribute
   * @TODO - put db functions into own class as 
   */
   public function update_magento_attribute($component, $attribute_info) {
    $id = db_update('webform_es_attribute')
      ->fields(array(
        'data' => $attribute_info,
      ))
      ->condition('mage_id',$component['extra']['magento_attribute_code'])
      ->execute();
   }

   /*
   * see if record exists into webform_es_attribute
   * @TODO - put db functions into own class as 
   */
   public function exists_magento_attribute($mage_id) {
    $query = db_select('webform_es_attribute', 'wea')
      ->fields('wea',array('mage_id'))
      ->condition('mage_id',$mage_id);
    $result = $query->execute()->fetch();
    if($result) {
      return true;
    } else {
      return false;
    }
   }
   
   public function get_stored_magento_attribute_data($mage_id) {
    $query = db_select('webform_es_attribute', 'wea')
      ->fields('wea',array('data'))
      ->condition('mage_id',$mage_id);
    $result = $query->execute()->fetch();
    if($result) {
      return $result;
    } else {
      return false;
    }
   }
}

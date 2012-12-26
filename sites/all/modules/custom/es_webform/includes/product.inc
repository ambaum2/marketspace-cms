<?php
class product {
  
  /*
   * take the array of attributes and return an 
   * array
   */
  public function create_attribute_select_options($attributes) {
    $options = array();
    foreach($attributes as $item) {
      $options[$item->code] = $item->code;
    }
    return $options;
  }
}

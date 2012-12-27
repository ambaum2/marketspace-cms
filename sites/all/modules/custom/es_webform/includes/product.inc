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
  /*
   * create a form input for a given magento 
   * response array
   * return array 
   */
  public function create_attribute_input($attribute) {
    $form_field = array(
      '#type' => $this->_convert_attribute_type($attribute['frontend_input']),
      '#title' => $component['name'],
      '#default_value' => $component['extra']['magento_attribute_code'],
      '#required' => $component['mandatory'],
    );
  }
  /*
   * convert magento attribute types to drupal types
   * return the drupal type name if found
   * otherwise default to textfield
   */
  public function _convert_attribute_type($type_name) {
    $types = array("textfield"=>"text",
      "select"=>"multiselect",
      "select"=>"boolean",
      "textfield"=>"weight",
      "date"=>"date",
      "textfield"=>"price",
      );
    if($key = array_search($type_name,$types)) {
      return $key;
    } else {
      return "textfield"; //default to textfield if nothing exists
    }
  }
}

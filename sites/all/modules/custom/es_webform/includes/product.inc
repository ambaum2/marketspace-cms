<?php
class product {
  
  /*
   * take the array of attributes from magento api method
   * catalogProductAttributeList(sessionId, attribute_set_id)
   *  and return an array
   */
  public function create_attribute_select_options($attributes) {
    $options = array();
    foreach($attributes as $item) {
      $options[$item->attribute_id] = $item->code;
    }
    asort($options); //sort alphabetically
    return $options;
  }
  /*
   * take the magento api catalogProductAttributeInfo
   * optiosn array (which is an array of objects)
   * and turn it into an array
   */
  private function _convert_attribute_options(Array $options_array) {
    $form_options = array();
    foreach($options_array as $item) {
      $form_options[$item->value] = $item->label;
    }
    return $form_options;
  }
  /*
   * create a form input for a given magento 
   * response array
   * return array 
   * @TODO : need to take set default value for editing
   * existing product if there is one
   */
  public function create_attribute_input($attribute) {
    $form_field = array(
      '#type' => $this->_convert_attribute_type($attribute->frontend_input),
      '#title' => $attribute->frontend_label[0]->label,
      //'#default_value' => $attribute->attribute_code,
      '#required' => ($attribute->is_required == 1) ? 'TRUE' : 'FALSE',
    );
    if(isset($attribute->options)) {
      $form_field['#options'] = $this->_convert_attribute_options($attribute->options);      
      if($attribute->frontend_input == "multiselect") {
        $form_field['#multiple'] = TRUE;
      }
    }
    foreach($attribute->additional_fields as $field) {
      if($field->key == 'is_wysiwyg_enabled' && $field->value == 1) {
        $form_field['#type'] = 'text_format'; //http://drupal.org/node/1031160#comment-6256154
        $form_field['#format'] = 'full_html';
      }
    }
    return $form_field;
  }

  /*
   * convert magento attribute types to drupal types
   * return the drupal type name if found
   * otherwise default to textfield
   */
  public function _convert_attribute_type($type_name) {
    $types = array(
      "text"=>"textfield",
      "multiselect"=>"select",
      "boolean"=>"select",
      "select"=>"select",
      "weight"=>"textfield",
      "date"=>"date",
      "price"=>"textfield",
      );
    $key = array_key_exists($type_name,$types);
    if($key = array_key_exists($type_name,$types)) {
      return $types[$type_name];
    } else {
      return "textfield"; //default to textfield if nothing exists
    }
  }
}

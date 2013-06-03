<?php
class magento_custom_options extends magento {
	var $session_id;
	function __construct() {
		parent::__construct();
		$this->session_id = parent::decryptText(variable_get('session_id'));
	}
	/*
	 * gets all custom option types, dropdown, 
	 * textfield, file etc. 
	 * recommended to use product_custom_options::
	 * getAllCustomOptions() instead for performance
	 * which has custom types hardcoded to avoid this 
	 * call
	 */
	public function getAllCustomOptionsTypes() {		
		$proxy = new SoapClient(magento::primary_host);
			try {
				$result = $proxy->catalogProductCustomOptionTypes($this->session_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
	/*
	 * api call to add custom option to a product
	 * $data will be an array of title, type, sort_order, 
	 * is_required, and aditional fields. Additional fields are:
	 * title (custom option title), price, price_type (fixed percent), 
	 * sku, max characters, sort_order, file_extension, 
	 * image_size_x (width limit for images), image_size_y, 
	 * value_id. For type dropdown can have multiple additional_fields
	 * 
	 * basic example of data array:
	 * $customTextFieldOption = array(
	    "title" => "Custom Text Field Option Title",
	    "type" => "field",
	    "is_require" => 1,
	    "sort_order" => 0,
	    "additional_fields" => array(
	        array(
	            "price" => 10.00,
	            "price_type" => "fixed",
	            "sku" => "custom_text_option_sku",
	            "max_characters" => 255
	        )
	    )
		);
	 */
	public function add_custom_option($product_id, $data, $store = 0) {		
		$proxy = new SoapClient(magento::primary_host);
			try {
				$result = $proxy->catalogProductCustomOptionAdd($this->session_id, $product_id, $data); //return true if added false if not
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
	/*
	 * update custom option by option id
	 * works similar to add_custom_option have to
	 * pass $data array in same format as it. 
	 * need only option_id. additional_fields in
	 * data array is optional
	 * 
	 * return boolean\int 1 if updated else 0
	 */
	public function update_custom_option($option_id, $data, $store = 0) {		
		$proxy = new SoapClient(magento::primary_host);
			try {
				$result = $proxy->catalogProductCustomOptionUpdate($this->session_id, $product_id, $data); //return true if added false if not
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
	/*
	 * gets custom option info for a given
	 *  option_id (int)
	 * returns an array similar to the add_custom_option 
	 * $data array used to create one
	 */
	public function get_custom_option_info($option_id, $store = 0) {		
		$proxy = new SoapClient(magento::primary_host);
			try {
				$result = $proxy->catalogProductCustomOptionInfo($this->session_id, $option_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
	/*
	 * gets all custom options for a given
	 * product by product_id (int). 
	 * returns an assoiative array
	 *  'option_id' => string '1' (length=1)
      'title' => string 'model' (length=5)
      'type' => string 'drop_down' (length=9)
      'is_require' => string '1' (length=1)
      'sort_order' => string '0' (length=1) 
	 */
	public function get_product_custom_options($product_id, $store = 0) {		
		$proxy = new SoapClient(magento::primary_host);
			try {
				$result = $proxy->catalogProductCustomOptionList($this->session_id, $product_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
	/*
	 * remove an option by option_id (string)
	 * returns true if removed false if not
	 */
	public function delete_product_custom_option($option_id) {		
		$proxy = new SoapClient(magento::primary_host);
			try {
				$result = $proxy->catalogProductCustomOptionRemove($this->session_id, $option_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
}

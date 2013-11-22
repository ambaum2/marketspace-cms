<?php
class magento_custom_options extends magento {
	var $session_key; //api session key
	function __construct() {
		parent::__construct();
		$this->session_key = parent::decryptText(variable_get('session_id'));
	}
  
	/**
	 * gets all custom option types, dropdown, 
	 * textfield, file etc. 
	 * recommended to use product_custom_options::
	 * getAllCustomOptions() instead for performance
	 * which has custom types hardcoded to avoid this 
	 * call
   * @return xml string
   *  xml string of custom option types
	 */
	public function getAllCustomOptionsTypes() {		
		$proxy = new SoapClient(variable_get("magento_api_wsdl"));
			try {
				$result = $proxy->catalogProductCustomOptionTypes($this->session_key);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
  
  /**
	 * simply add a new custom option by
	 * passing the product id
   * @param product_id | integer 
   *  product id of product for this custom option in magento
   * @param data | array
   *  keyed array of option parameters
   * @param store | integer
   *  the id of the store this product and option belong to (we don't use multistore setup)
   * @return boolean 
   *  true if worked false otherwise
	 */
	public function add_custom_option($product_id, $data, $store = 0) {		
		$proxy = new SoapClient(variable_get("magento_api_wsdl"));
			try {
				$result = $proxy->catalogProductCustomOptionAdd($this->session_key, $product_id, $data); //return true if added false if not
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
	
	/**
	 * update custom option by option id
	 * works similar to add_custom_option have to
	 * pass $data array in same format as it. 
	 * need only option_id. additional_fields in
	 * data array is optional
   * 
   * @param option_id | integer
   *  option unique id (every option has unique id) in magento
   * @param data | array
   *  keyed array of option parameters
   * @param store | integer
   *  the id of the store this product and option belong to (we don't use multistore setup)
   * @return boolean 
   *  true if option updated false otherwise
	 * return boolean\int 1 if updated else 0
	 */
	public function update_custom_option($option_id, $data, $store = 0) {		
		$proxy = new SoapClient(variable_get("magento_api_wsdl"));
			try {
				$result = $proxy->catalogProductCustomOptionUpdate($this->session_key, intval($option_id), $data); //return true if added false if not
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
  
	/**
	 * gets custom option info for a given
	 *  option_id (int)
	 * returns an array similar to the add_custom_option 
	 * $data array used to create one
   * @param option_id | integer
   *  option unique id (every option has unique id) in magento
   * @param store | integer
   *  the id of the store this product and option belong to (we don't use multistore setup)
   * @return boolean 
   *  true if option updated false otherwise
	 */
	public function get_custom_option_info($option_id, $store = 0) {		
		$proxy = new SoapClient(variable_get("magento_api_wsdl"));
			try {
				$result = $proxy->catalogProductCustomOptionInfo($this->session_key, $option_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
  
	/**
	 * gets all custom options for a given
	 * product by product_id (int). 
   * @param product_id | integer 
   *  product id of product for this custom option in magento
   * @param store | integer
   *  the id of the store this product and option belong to (we don't use multistore setup)
   * @return array
	 *   returns an assoiative array
	 *  'option_id' => string '1' (length=1)
   *   'title' => string 'model' (length=5)
   *   'type' => string 'drop_down' (length=9)
   *   'is_require' => string '1' (length=1)
   *   'sort_order' => string '0' (length=1) 
	 */
	public function get_product_custom_options($product_id, $store = 0) {		
		$proxy = new SoapClient(variable_get("magento_api_wsdl"));
			try {
				$result = $proxy->catalogProductCustomOptionList($this->session_key, $product_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
  
	/**
	 * remove an option by option_id (string)
   * @param option_id | integer
   *  option unique id (every option has unique id) in magento
	 * @return 
   *  returns true if removed false if not
	 */
	public function delete_product_custom_option($option_id) {		
		$proxy = new SoapClient(variable_get("magento_api_wsdl"));
			try {
				$result = $proxy->catalogProductCustomOptionRemove($this->session_key, $option_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
  
	/**
	 * remove a custom option value
	 * so existing value have to
   * @param value_id | integer
   *  option value unique id (every option value has unique id) in magento
   * @param store | integer
   *  the id of the store this product and option belong to (we don't use multistore setup)
   * @return boolean 
	 *   returns boolean - true if removed 
	 */
	public function remove_custom_option_value($value_id, $store = 0) {		
		$proxy = new SoapClient(variable_get("magento_api_wsdl"));
			try {
				$result = $proxy->catalogProductCustomOptionValueRemove($this->session_key, $value_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
}

<?php
class magento_product_types_sets extends magento {
	var $session_id;
	function __construct() {
		parent::__construct();
		$this->session_id = parent::decryptText(variable_get('session_id'));
	}
	/*
	 * get all products attribute sets
	 */
	public function getAllAttributeSets() {		
		$proxy = new SoapClient(magento::primary_host);
			try {
				$result = $proxy->catalogProductAttributeSetList($this->session_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}  
	/*
	 * get all products types
	 */
	public function getAllProductTypes() {		
		$proxy = new SoapClient(magento::primary_host);
			try {
				$result = $proxy->catalogProductTypeList($this->session_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}  
	/*
	 * convert product types result object to 
	 * an array for form api
	 */
	public function convertTypesToOptionsArray($types) {
		$options = array();
		foreach($types as $type) {
			$options[$type->type] = $type->label;
		}
		return $options;
	}
	/*
	 * convert product attributes set result object to 
	 * an array for form api
	 */
	public function convertProductAttributesToOptionsArray($sets) {
		$options = array();
		foreach($sets as $set) {
			$options[$set->set_id] = $set->name;
		}
		return $options;
	}
}
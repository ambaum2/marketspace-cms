<?php
class magento_custom_options extends magento {
	var $session_id;
	function __construct() {
		parent::__construct();
		$this->session_id = parent::decryptText(variable_get('session_id'));
	}
	
	public function getAllCustomOptionsTypes() {		
		$proxy = new SoapClient(magento::primary_host);
			try {
				$result = $proxy->catalogProductCustomOptionTypes($this->session_id);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
}

<?php
class magento_product extends magento {
	var $session_key;
	function __construct() {
		parent::__construct();
		$this->session_key = parent::decryptText(variable_get('session_id'));
	}
	
	/**
	 * get all products list using filter
   * @param complex_filter | array
   *  array to filter out product by whatever parameters
   * @return xml string
   *  returns products from magento as xml
	 * @TODO this is limited you should consider creating your own
	 * webservice method
	 */
	public function getAllUsersProducts($complex_filter = null) {		
		$proxy = new SoapClient(variable_get("magento_api_wsdl"));
			try {
				$result = $proxy->catalogProductList($this->session_key, $complex_filter);
				//drupal_set_message('<xmp>' . $proxy->__getLastRequest() . '</xmp>', 'notice');
				//dpm('<xmp>' . $proxy->__getLastRequestHeaders() . "</xmp>");
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
  
  /**
   * get magento product by id
   * @param product_id | integer
   *  product id from magento
   * @param additional_attributes | array
   *  custom attributes (non core) from magento that were made
   * @return xml string
   *  product xml
   */
  public function getProductById($product_id, $additional_attributes) {   
    $proxy = new SoapClient(variable_get("magento_api_wsdl"));
      try {
        $result = $proxy->catalogProductInfo($this->session_key, $product_id, null, $additional_attributes);//array('additional_attributes'=>array('hide_dining_sweets', 'vendor_email')));
        return $result;
      } catch(exception $e) {
        throw $e;
      }
    }
  
	/**
	 * get all products attribute sets
   * @return xml string
   *  attribute set xml data in string 
	 */
	public function getAllAttributeSets() {		
		$proxy = new SoapClient(variable_get("magento_api_wsdl"));
			try {
				$result = $proxy->catalogProductAttributeSetList($this->session_key);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
  
	/**
	 * get all products types
   * @return xml string
   *  product types as xml string 
	 */
	public function getAllProductTypes() {		
		$proxy = new SoapClient(variable_get("magento_api_wsdl"));
			try {
				$result = $proxy->catalogProductTypeList($this->session_key);
				return $result;
			} catch(exception $e) {
				throw $e;
			}
		}
  
  /**
   * update a product product
   * @param product_id | integer 
   *  product id of product for this custom option in magento
   * @param data | array
   *  keyed array of option parameters
   * @return boolean 
   *  true if worked false otherwise
   */
  public function updateProduct($product_id, $data) {
     $proxy = new SoapClient(variable_get("magento_api_wsdl"));
     try {    
        $result = $proxy->catalogProductUpdate($this->session_key, $product_id, $data);
        return $result;
     } catch (exception $e) {
        throw $e;
     }
  }
  
  /**
   * update a product stock data - only update function exists
	 * b/c stock data is in the core
   * @param product_id | integer 
   *  product id of product for this custom option in magento
   * @param data | array
   *  keyed array of option parameters
   * @return boolean 
   *  true if worked false otherwise
   */
  public function updateProductStockData($product_id, $data) {
     $proxy = new SoapClient(variable_get("magento_api_wsdl"));
     try {    
        $result = $proxy->catalogInventoryStockItemUpdate($this->session_key, $product_id, $data);
        return $result;
     } catch (exception $e) {
        drupal_set_message(t("Error could not update/add product inventory information. Contact admin"), 'error');
        watchdog('magento', 'update product stock data <pre>%e</pre>', array('%e' => print_r($e,true),WATCHDOG_ERROR));
        throw $e;
     }
  }
  
  /**
   * update a product stock data - only update function exists
   * b/c stock data is in the core
   * you can pass mulitple product ids but not useful to me now
   * returns a keyed array right now only useful for me to get one
   * @param product_id | integer 
   *  product id of product for this custom option in magento
   * @return xml string 
   *  return stock data as xml string
   */
  public function getProductStockData($product_id) {
     $proxy = new SoapClient(variable_get("magento_api_wsdl"));
     try {    
        $result = $proxy->catalogInventoryStockItemList($this->session_key, array($product_id));
        return $result;
     } catch (exception $e) {
        drupal_set_message(t("Error could not get product inventory information. Contact admin"), 'error');
        watchdog('magento', 'get product stock data <pre>%e</pre>', array('%e' => print_r($e,true),WATCHDOG_ERROR));
        throw $e;
     }
  }

	/**
	 * check if element is built in magento attribute 
	 * by attribute code
	 * @param attribute_code | string
   *  attribute code which is some string
   * @return boolean
   *  true if core attribute false otherwise
	 * removed: 'special_price','special_from_date'
      ,'special_to_date'
	 */
	public function isCoreMagentoAttribute($attribute_code) {
    $default_magento_attributes = array('categories','websites','name','description','short_description','weight','status','url_key','url_path'
      ,'visibility','category_ids','website_ids','has_options','gift_message_available','price'
      ,'tax_class_id','meta_title','meta_keyword','meta_description','custom_design','custom_layout_update','options_container'
      );
    $default_magento_stock_attributes = array('qty','is_in_stock','manage_stock','use_config_manage_stock','min_qty','use_config_manage_stock'
      ,'min_qty','use_config_min_qty','min_sale_qty','use_config_min_sale_qty',' max_sale_qty',' use_config_max_sale_qty','');
		if(in_array($attribute_code, $default_magento_attributes) || in_array($attribute_code, $default_magento_stock_attributes)) {
			return true;
		} else {
			return false;
		}
	}
  
	/**
	 * TODO - should probably be static method
   * @return array
   *  return array of core stock attributes
	 */
	public function getCoreStockAttributes() {
		  return array('qty','is_in_stock','manage_stock','use_config_manage_stock','min_qty','use_config_manage_stock'
      ,'min_qty','use_config_min_qty','min_sale_qty','use_config_min_sale_qty',' max_sale_qty',' use_config_max_sale_qty','');
	}
}

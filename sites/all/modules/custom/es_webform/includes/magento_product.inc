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
	 * get nid by product attribute set id
   * @param  attribute_set_id | integer
   *    the attribute set id for a set in magento
   * @param product_type | string
   *  the product type
   * @return array
   *    an array of entity id, attribute set id, adn type_code
	 */
	public function getWebformIdByAttributeSetAndType($attribute_set_id,$product_type) {

    $query = db_select('field_data_field_market_space_setup', 'a')
      ->fields('a',array('entity_id', 'field_market_space_setup_attribute_set_id', 'field_market_space_setup_type_code'))
      ->condition('field_market_space_setup_attribute_set_id', (int)$attribute_set_id)
			->condition('field_market_space_setup_type_code', $product_type);
    $query->join('field_data_field_tickettype', 't', 't.entity_id = a.entity_id');
    $query->fields('t',array('field_tickettype_value'));
    $node_webform = $query->execute()->fetchAll();
		return $node_webform;
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

  /**
   * process update/create product data
   * from a submission array
   * return data array for update/insert 
   * api call
   * 
   * note the returned array will have
   * an array called inventory_data 
   * where inventory attributes are stored
   * since they are handled seperatly from
   * attributes and additional_attributes
   * @param form_state | array
   *  form state for submitted form
   * @param insert | boolean
   *  true if inserting false if updating
   * @return array
   *  returns product data as array for confirmation
   *
   * TODO Change submission to your array
   * TODO make a seperate function for getting
   * stock data array
   */
  public function getProductDataArray($form_state, $insert=false) {
    $magento_product_attributes = new magento_product_attributes();
    $magento_additional_attributes = array();
    $magento_product_data = array();
		$input = $form_state['input'];
		//get webform data for processing input
		$node = node_load($form_state['input']['product_edit_node_id']);
		$webform = $node->webform["components"];
    foreach($webform as $item=>$value) {
      //is this a magento component whether custom (custom options, categories, images etc) or built in
      if(isset($webform[$item]['extra']['magento_attribute_code'])) {
        $mage_id = $webform[$item]['extra']['magento_attribute_code'];
        //if it is a standard catalog attribute default or custom attribute
        if($attribute_data = $magento_product_attributes->get_stored_magento_attribute_data($mage_id)) {
	        $attribute = unserialize($attribute_data->data); //get stored attribute data from drupal
	        if($attribute->frontend_input == "textarea") { //could also check ->additional_fields[0]->is_wysiwyg_enabled
	          if($this->isCoreMagentoAttribute($attribute->attribute_code)) {//in_array($attribute->attribute_code,$default_magento_attributes)) {
	              $magento_product_data[$attribute->attribute_code] = $input[$attribute->attribute_code]['value'];
	          } else { //custom attribute so put in additional attributes
	              $magento_additional_attributes['single_data'][] = array('key'=>$attribute->attribute_code, 'value'=>$input[$attribute->attribute_code]['value']);
	          }
	        } else { //for all other types 
		          if($attribute->frontend_input == "multiselect") {
		              $selections = array();
		              foreach($input[$attribute->attribute_code] as $select_key=>$select_value) {
		                $selections[] = $select_value;
		              } 
		              if($this->isCoreMagentoAttribute($attribute->attribute_code)) { //in_array($attribute->attribute_code,$default_magento_attributes)) {
		                $magento_product_data[$attribute->attribute_code] = $selections;
		              } else {
		                $magento_additional_attributes['multi_data'][] = array('key'=>$attribute->attribute_code, 'value'=>$selections);
		              }
		          } else { //textfields, number, should be processed the same
		              if($this->isCoreMagentoAttribute($attribute->attribute_code)) {
		                  $magento_product_data[$attribute->attribute_code] = $input[$attribute->attribute_code];
		              } else {
		                  $magento_additional_attributes['single_data'][] = array('key'=>$attribute->attribute_code, 'value' => $input[$attribute->attribute_code]);
		              }
		          }
	        }
	      //all other attributes must be exlplicitely referenced
      } //if not then a regular attribute could be category or inventroy
    } elseif(isset($webform[$item]['extra']['inventory_attribute'])) {
    	 $magento_product_data['inventory_data'][$webform[$item]['extra']['inventory_attribute']] = $input[$webform[$item]['extra']['inventory_attribute']];
       if(!isset($magento_product_data['inventory_data']['manage_stock'])) { // automatically set manage stock to true if any inventory attribute is used
         $magento_product_data['inventory_data']['manage_stock'] = 1;
       }
    } elseif(isset($webform[$item]['extra']['magento_categories'])) {      
	      $categories = array();
	      foreach($input['categories'] as $cat_key=>$cat_value) {
	        if($cat_value == 1) {
	          $categories[] = $cat_key;
	        }
	      }
	      $magento_product_data['categories'] = $categories;
  	} else {
  		
  	}
  }
	$magento_product_data['additional_attributes'] = $magento_additional_attributes;
	return $magento_product_data;
  }
}

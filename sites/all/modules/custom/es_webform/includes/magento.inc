<?php
class magento {
  private static $mage_singleton = null;
  const api_user = "market_space_user"; //@TODO : should not be constat?
  const api_password = "19e16eb11791a0a05033ada1baafc51c621c2abb "; //also should not? keep the space
  private $encrypt_key = "b4baa5e8c5e5676db4baa5e";//8c5e5676d
  /*
   * constructor to login and start session
   */
   function __construct() {
      $session_start_time = variable_get("session_start");
      if(!empty($session_start_time)) { //if a session is active
        if((time() - variable_get("session_start")) > 0) { //login again session has been active for more than 20 minutes
          $this->login();
        }
      } else { //login if session has not been started yet
          $this->login();    
      } 
  }

  /**
   * login to specified host
   * set session start and session id in system var (not good!!!!)
   * @return session_key
   *  string that is a hash value 
   * @TODO : set hosts api users session ids in a database column
   */
  private function login() {  
    $proxy = new SoapClient(variable_get("magento_api_wsdl"));
    try {
      $session_key = $proxy->login(magento::api_user, magento::api_password); // @TODO : change login and pwd if necessary
      variable_set("session_start",time());//set session start time   
      $encrypted_session_id = $this->encryptText($session_key);
      variable_set("session_id", $encrypted_session_id); //set session login id
    } catch(exception $e) {
      throw $e;
    }
    return $session_key;
  }
  
/**
 * get all attributes for an attributes set for some magento install
 * @param  attribute_set_id
 *    the attribute set id for a set in magento
 * @param session_key 
 *    the api session_key
 * @return array
 *    an array keyed by .. @todo add returned keys
 */
  public function get_all_attributes($session_key, $attribute_set_id) {
    $proxy = new SoapClient(variable_get("magento_api_wsdl"));
		
    try {
      $result = $proxy->catalogProductAttributeList($session_key, $attribute_set_id);
      return $result;
    } catch (exception $e) {
      throw $e;
    }
  }
  /**
   * get information about an attribute. Its schema, default values,
   * required or not, input type, etc
   * @param session_key | string
   *  the api session key
   * @param attribute_code | string
   *  the attribute code for a magento attribute
   * @return array
   *    a string of xml. keyed by ...
   */
  public function get_attribute_information($session_key, $attribute_code) {
    $proxy = new SoapClient(variable_get("magento_api_wsdl"));
    try {
      $result = $proxy->catalogProductAttributeInfo($session_key, $attribute_code);
      return $result;
    } catch (exception $e) {
      throw $e;
    }
  }
  
  /**
   * create product in magento
   * @param session_key | string
   *  the api session key
   * @param type | string 
   *  the type of product from magento (virtual, bundle etc)
   * @param attribute_set | string
   *  the attribute set type from magento
   * @param data | array
   *  a keyed array of data for the created product
   * @return result | boolean
   *  returns true if success false otherwise
   */
   public function create_product($session_key, $type, $attribute_set, $sku, $data) {
     $proxy = new SoapClient(variable_get("magento_api_wsdl"));
     try {  	
     		$result = $proxy->catalogProductCreate($session_key, $type, $attribute_set, $sku, $data);
     		return $result;
  	 } catch (exception $e) {
  		  throw $e;
  	 }
  }
  /** 
   * encrypt data for storage in end user cookies
   * with triple des
   * @param plainText | string
   *  string of text to encrypt
   * @return string 
   *  base 64 encoded string
   */
  function encryptText($plainText) {
    $encText = mcrypt_encrypt(MCRYPT_TRIPLEDES, $this->encrypt_key, $plainText,MCRYPT_MODE_ECB);
    return base64_encode($encText);
  }
  /**
   * unencrypt end user cookie data 
   * @param encryptText | string
   *  base64 encoded and ecb triple des encoded 
   * @return string
   *   decoded string
   */
  function decryptText($encryptText) {
    $cipherText = base64_decode($encryptText);
    $res = mcrypt_decrypt(MCRYPT_TRIPLEDES, $this->encrypt_key, $cipherText, MCRYPT_MODE_ECB);
    return $res;
  }
}

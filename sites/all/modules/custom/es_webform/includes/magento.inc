<?php
class magento {
  private static $mage_singleton = null;
  const api_user = "market_space_user"; //@TODO : should not be constat?
  const api_password = "19e16eb11791a0a05033ada1baafc51c621c2abb "; //also should not? keep the space
  private $encrypt_key = "b4baa5e8c5e5676db4baa5e";//8c5e5676d
  const primary_host = "http://mspace.thedotworldgroup.com/index.php/api/v2_soap?wsdl";
  public $base_url = "http://mspace.thedotworldgroup.com/index.php/";
  /*
   * constructor to login and start session
	 * TODO make this a singleton again
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

  /*
   * login to specified host
   * set session start and session id in client side cookies
   * @TODO : set hosts api users session ids in a database column
   * and update session id every twenty minutes on cron
   */
  private function login($host = magento::primary_host) {  
    $proxy = new SoapClient(magento::primary_host);
    try {
      $sessionId = $proxy->login(magento::api_user, magento::api_password); // @TODO : change login and pwd if necessary
      variable_set("session_start",time());//set session start time   
      $encrypted_session_id = $this->encryptText($sessionId);
      variable_set("session_id", $encrypted_session_id); //set session login id
    } catch(exception $e) {
      throw $e;
    }
    return $sessionId;
  }
  
  /*
   * get all attributes for an attributes set for some magento install
   * return array
   */
  public function get_all_attributes($attribute_set_id, $session_id, $host = magento::primary_host) {
    $proxy = new SoapClient($host);
		
    try {
      $result = $proxy->catalogProductAttributeList($session_id, $attribute_set_id);
      return $result;
    } catch (exception $e) {
      throw $e;
    }
  }
  /*
   * get information about an attribute. Its schema, default values,
   * required or not, input type, etc
   * return array
   */
  public function get_attribute_information($session_id, $attribute_code, $host = magento::primary_host) {
    $proxy = new SoapClient($host);
    try {
      $result = $proxy->catalogProductAttributeInfo($session_id, $attribute_code);
      return $result;
    } catch (exception $e) {
      throw $e;
    }
  }
  
  /*
   * create product in magento
   */
   public function create_product($session_id, $type, $attribute_set, $sku, $data, $host = magento::primary_host) {
     $proxy = new SoapClient($host);
     try {  	
     		$result = $proxy->catalogProductCreate($session_id, $type, $attribute_set, $sku, $data);
     		return $result;
  	 } catch (exception $e) {
  		  throw $e;
  	 }
  }
  /* 
   * encrypt data for storage in end user cookies
   * with triple des
   */
  function encryptText($plainText) {
    $encText = mcrypt_encrypt(MCRYPT_TRIPLEDES, $this->encrypt_key, $plainText,MCRYPT_MODE_ECB);
    return base64_encode($encText);
  }
  /*
   * unencrypt end user cookie data 
   */
  function decryptText($encryptText) {
    $cipherText = base64_decode($encryptText);
    $res = mcrypt_decrypt(MCRYPT_TRIPLEDES, $this->encrypt_key, $cipherText, MCRYPT_MODE_ECB);
    return $res;
  }
}

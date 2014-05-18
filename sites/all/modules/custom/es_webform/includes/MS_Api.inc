<?php
class MS_Api {
	var $secret = "a42342963283bb395a0430346e4d49ad";

	/**
	 * decrypt data
	 * data will come over url encoded 
	 * but no need to urldecode that happens
	 * automatically with arg
	 * @param string 
	 * 	base64 encoded string encrypted 
	 * 	with ecb
	 * @return string
	 * return decrypted string
	 */
	public function decryptBase64($text, $iv, $key = "18a1a224151413a53056b609a85d1085") { 
	  return trim(mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, base64_decode($text), MCRYPT_MODE_CBC, $iv));

	}
	
	/**
	 * urlencrypt data
	 *
	 * @param data | mixed
	 * @return string
	 * 	returns a base 64 encoded string from secret key
	 */
	public function encryptBase64($text, $iv, $key = "18a1a224151413a53056b609a85d1085") { 
	  return trim(base64_encode(mcrypt_encrypt(MCRYPT_RIJNDAEL_128, $key, $text, MCRYPT_MODE_CBC, $iv)));	  	
	}
	
  /**
   * create iv for encryption
   * @return string
   *  return an iv 
   */
  public function createIv() {
    return mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_CBC), MCRYPT_DEV_URANDOM);
  }
	/**
	 * make an api call to magento 
	 * only handles get right now
	 * @param path | string
	 * 	api path
	 * @return array
	 * 	json decoded array
	 */
	public function apiTokenCall($path, $object = true) {
        $time = time();
        $text = $this->secret . "|" . $time;

        $iv = $this->createIv();
        $encryptedText = $this->encryptBase64($text, $iv);
        $ivBase64 = base64_encode($iv);
        //$url = str_replace("phpunit/", "", Mage::getBaseUrl() . "mspaceapi/product/v1/attribute/type/options/code/product_type");
        $url = variable_get("magento_base_url") . $path;
        $handle = curl_init();
        $headers = array("Content-Type: application/json", "authtoken:$encryptedText", "authiv:$ivBase64");
        curl_setopt($handle, CURLOPT_URL, $url);
        curl_setopt($handle, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($handle, CURLOPT_SSL_VERIFYPEER, false);
        //curl_setopt($handle, CURLOPT_USERPWD, $username . ":" . $password);
        curl_setopt($handle, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($handle, CURLINFO_HEADER_OUT, true); //can be taken out in production only needed for curl_getinfo
        $data = curl_exec($handle);
        curl_close($handle);

        $data = json_decode($data, $object);
        return $data;
    }
}
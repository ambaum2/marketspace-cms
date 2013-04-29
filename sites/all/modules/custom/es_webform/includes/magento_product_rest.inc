<?php
class magento_product_rest {
  const api_user = "marketspace"; //@TODO : should not be constat?
  //const api_password = "19e16eb11791a0a05033ada1baafc51c621c2abb "; //also should not? keep the space
  const consumerKey = "0yhjtykckqhbs6h3j4atkgw5ofc8c51u"; //key: 0yhjtykckqhbs6h3j4atkgw5ofc8c51u
  const consumerSecret = "pk7u0854g6za0e2bmxl5oau50pcc2c5d";//secret: pk7u0854g6za0e2bmxl5oau50pcc2c5d
  private $api_url = "http://aaortho.net/api/rest";
  private $encrypt_key = "b4baa5e8c5e5676db4baa5e";//8c5e5676d
	private $callbackUrl = "http://aaortho.net/oauth_admin.php";
	private $adminAuthorizationUrl = 'http://aaortho.net/admin/oauth_authorize';
	private $accessTokenRequestUrl = 'http://aaortho.net/oauth/token';
	private $oauth_callback_url = "http://aaortho.net/oauth/initiate?oauth_callback=";
  
  /*
   * constructor to login and start session
   */
   function __construct() {
      $session_start_time = variable_get("oauth_session_start");
			$session_started = false; //if request token works then have to get access token
      if(!empty($session_start_time)) { //if a session is active
        if((time() - variable_get("oauth_session_start")) > 0) { //login again session has been active for more than 20 minutes        
          $session_started = $this->getRequestToken();
        } 
      } else { //login if session has not been started yet
          $session_started = $this->getRequestToken();    
      } 
			if($session_started) {
				$this->setToken();
			}
  }
	
	/*
	 * get request token 
	 */
	function getRequestToken() {
		$oauthClient = new OAuth(magento_product_rest::consumerKey, magento_product_rest::consumerSecret, OAUTH_SIG_METHOD_HMACSHA1, OAUTH_AUTH_TYPE_URI);
		$temporaryCredentialsRequestUrl = $this->oauth_callback_url . urlencode($this->callbackUrl);
		try {
			$requestToken = $oauthClient->getRequestToken($temporaryCredentialsRequestUrl);
			variable_set("oauth_session_start",time());
			echo "<pre>" . print_r($requestToken,true) . "</pre>";
			variable_set("oauth_token_secret", $requestToken['oauth_token_secret']);
			variable_set("oauth_token", $requestToken['oauth_token']);
			$ch = curl_init();
			curl_setopt($ch, CURLOPT_URL, $this->adminAuthorizationUrl . '?oauth_token=' . $requestToken['oauth_token']);
			curl_setopt($ch, CURLOPT_HEADER, 0);
			//curl_exec($ch);
			echo "<br />" . $this->adminAuthorizationUrl . '?oauth_token=' . $requestToken['oauth_token'] . "<br />";
			curl_close($ch);
			return true;
		} catch(exception $e) {
			throw $e;
		}
		return false;
	}
	/*
	 * set token 
	 */
	function setToken() {
		$oauthClient = new OAuth(magento_product_rest::consumerKey, magento_product_rest::consumerSecret, OAUTH_SIG_METHOD_HMACSHA1, OAUTH_AUTH_TYPE_URI);
		$oauthClient->enableDebug();
		try {
			$oauthClient->setToken(variable_get("oauth_token"), variable_get("oauth_token_secret"));
			echo variable_get("oauth_token") . " secret:" .variable_get("oauth_token_secret");
			$accessToken = $oauthClient->getAccessToken("http://aaortho.net/oauth/token/");
			variable_set("rest_token", $accessToken['oauth_token']);
			variable_set("rest_secret", $accessToken['oauth_token_secret']);
			return true;
		} catch(exception $e) {
			throw $e;
		} 
		return false;
	}
	/*
	 * get products
	 */
	function getProducts() {
		$oauthClient = new OAuth(magento_product_rest::consumerKey, magento_product_rest::consumerSecret, OAUTH_SIG_METHOD_HMACSHA1, OAUTH_AUTH_TYPE_AUTHORIZATION);
		try {
			$oauthClient->setToken(variable_get("rest_token"), variable_get("rest_secret"));
			$resourceUrl = $this->api_url . "/products";
			$oauthClient->fetch($resourceUrl);
			$productsList = json_decode($oauthClient->getLastResponse());
			return $productsList;			
		} catch(exception $e) {

		}
	}
}
<?php
class MS_CRM_Api implements iMS_Api {
    public $postBody; /** @var $postBody array */
    public $parameters; /** @var parameters array of url params key => value */

    /**
     * make an api call to magento
     * only handles get right now
     * @param $path
     * @param bool $array
     * @internal param $ path | string
     *    api path
     * @return array
     *    json decoded array
     */
    public function request($path, $array = false) {
        $url = variable_get("ms_crm_constant_contact_endpoint") . $path . 'api_key=' . variable_get("ms_crm_constant_contact_key"); //4b34b4bf-3903-437c-9fe0-0cf12c72aec1 token
        $handle = curl_init();
        $headers = array("Content-Type: application/json", "Authorization: Bearer " . variable_get("ms_crm_constant_contact_token"));
        curl_setopt($handle, CURLOPT_URL, $url);
        curl_setopt($handle, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($handle, CURLOPT_SSL_VERIFYPEER, false);
        //curl_setopt($handle, CURLOPT_USERPWD, $username . ":" . $password);
        curl_setopt($handle, CURLOPT_RETURNTRANSFER, true);
        //curl_setopt($handle, CURLINFO_HEADER_OUT, true); //can be taken out in production only needed for curl_getinfo
        $data = curl_exec($handle);
        curl_close($handle);

        $data = json_decode($data, $array);
        return $data;
    }

    public function PostRequest($path, $array = false) {
        $url = variable_get("ms_crm_constant_contact_endpoint") . $path . 'api_key=' . variable_get("ms_crm_constant_contact_key"); //4b34b4bf-3903-437c-9fe0-0cf12c72aec1 token
        $handle = curl_init();
        $headers = array("Content-Type: application/json", "Authorization: Bearer " . variable_get("ms_crm_constant_contact_token"));
        curl_setopt($handle, CURLOPT_URL, $url);
        curl_setopt($handle, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($handle, CURLOPT_POST, true);
        curl_setopt($handle, CURLOPT_POSTFIELDS, $this->postBody);
        curl_setopt($handle, CURLOPT_SSL_VERIFYPEER, false);
        curl_setopt($handle, CURLOPT_RETURNTRANSFER, true);
        //curl_setopt($handle, CURLOPT_VERBOSE, true);
        $data = curl_exec($handle);
        $response_code = curl_getinfo($handle, CURLINFO_HTTP_CODE);
        if($response_code === '201') {
            $data = json_decode($data, $array);
        } else {
            $data = array('error' => array('code' => $response_code, 'details' => json_decode($data, $array)));
        }
        curl_close($handle);
        return $data;
    }

    public function SetPostBody() {
        $fields = array();
        foreach($this->postBody as $key => $value) {
            $fields[$key] = urlencode($value);
        }
        return $fields;
    }
    /**
     * make the request
     * @return string $params_string
     */
    public function setParameters()
    {
        $params_string = "?";
        if(isset($this->parameters)) {
            foreach($this->parameters as $key => $value) {
                $params_string .= "$key=$value&";
            }
        }
        return $params_string;
    }
}
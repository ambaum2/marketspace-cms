<?php
require 'PHPUnit/Autoload.php';
class TestApi extends PHPUnit_Framework_TestCase
{
    public $dpl_dir = '/var/www/mspacecms/';
    const DRUPAL_ROOT = '/var/www/mspacecms/';
    const REMOTE_ADDR = '162.243.110.111';
    public $api;
    public function __construct() {
        $_SERVER['HTTP_HOST'] = basename($this->dpl_dir);
        $_SERVER['REMOTE_ADDR'] = '127.0.0.1';
        define('DRUPAL_ROOT', $this->dpl_dir);
        set_include_path($this->dpl_dir . PATH_SEPARATOR . get_include_path());
        require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
        drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
        $this->api = new MS_CRM_Api();
    }
    public function testRequest() {
        $result = $this->api->request('account/info');
        $this->assertTrue(!empty($result));
        //print_r($result);
    }

    public function testGetContacts() {
        $query_strings = array(
            '?limit=1&' => array('params' => array('limit' => 1)),
            '?limit=2&' =>array('params' => array('limit' => 2)),
            '?limit=2&email=crmaresh@gmail.com&' =>array('params' => array('limit' => 2, 'email' => 'crmaresh@gmail.com')),
        );
        /**
         * integration test
         */
        foreach($query_strings as $param_string => $params) {
            $contacts = new MS_Individuals_Contacts($this->api, 'contacts', $params['params']);
            $this->assertEquals($param_string, $contacts->getQueryString());
            $result = $contacts->get();
            $this->assertLessThanOrEqual($params['params']['limit'], count($result->results));
            //print_r($result);
        }
    }
    public function testGetEventRecipients() {
        $cc = new MS_Constant_Contact_Admins();
        $email_addresses = $cc->GetEventRecipients();
        print_r($email_addresses);
    }
    public function testCreateContact() { //first guy created's id was 1365683154
        $params = array(
            array('params' => array('action_by' => 'ACTION_BY_VISITOR')), //'?action_by=ACTION_BY_VISITOR&' =>
        );
        $contact = new MS_Individuals_Contacts($this->api, 'contacts', $params[0]['params']);
        $contact->postBody = array(
            "first_name" => "Ronald",
            "last_name" => "Martone",
            "lists" => array(
                [
                    "id" => "1966704516", //you'll need to hardcode this - a list is required
                ],
            ),
            "email_addresses" => array(
                array("email_address" => "al2@thedotworldgroup.com"), //api max length 80
            ),
            "notes" => array(
                [
                    "note" => "some note", //api's max length is 500
                ],
            ),
        );
        //print_r($contact->postBody);
        $this->assertEquals('?action_by=ACTION_BY_VISITOR&', $contact->getQueryString());
        $result = $contact->post();
        print_r($result);
    }
}
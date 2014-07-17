<?php
require 'PHPUnit/Autoload.php';
class TestCCSubmission extends PHPUnit_Framework_TestCase
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
    public function testGetUnserializedSubmission() {
        $submission = $this->getSubmissionTestData();
        $cc = new MS_Constant_Contact_Contacts_Submission($submission);
        $result = $cc->GetUnserializedSubmission();
        print_r($result);
        $expected = array(3 => false, 4=> false, 6 => true, 2 => true, 5 => true);
        foreach($result->data as $cid => $value) {
            foreach($value['value'] as $delta => $val) {
                $this->assertEquals($expected[$cid], is_array($val)); //is item an array - so did unserialization work properly
            }
        }
    }
    public function testGetSubmissionValue() {
        $value = array(
            0 => array(0 => array('email_addresses' => array(0 => array('email_address' => 'foo@foo.com'))), true),
            1 => array(0 => array('first_name' => 'alan'), false),
        );
        $cc = new MS_Constant_Contact_Contacts_Submission();
        //print_r($value);
        foreach($value as $key => $field) {
            $result = $cc->GetSingleSubmissionValue($field[0]);
            //print_r($result);
            $this->assertEquals($field[1], is_array($result));
        }
    }
    public function testGetSubmission() {
        $submission = $this->getTestData();
        $node = node_load($submission->nid);
        $cc = new MS_Constant_Contact_Contacts_Submission($submission, $node);
        $result = $cc->GetSubmission();
        $this->assertEquals(2, count($result));
        //print_r($result);
    }

    public function getSubmissionTestData() {
        return (object)(array( 'nid' => '28', 'uid' => '1', 'submitted' => 1405546860, 'remote_addr' => '64.90.21.130', 'is_draft' => 0, 'data' => array ( 3 => array ( 'value' => array ( 0 => 'asdf', ), ), 4 => array ( 'value' => array ( 0 => '3142926585', ), ), 6 => array ( 'value' => array ( 'email_addresses' => 'a:1:{i:0;a:1:{s:13:"email_address";s:4:"asdf";}}', ), ), 2 => array ( 'value' => array ( 'addresses' => 'a:1:{i:0;a:8:{s:12:"address_type";s:8:"BUSINESS";s:4:"city";s:11:"saint louis";s:12:"country_code";s:2:"US";s:5:"line1";s:14:"401 n lindberg";s:5:"line2";s:0:"";s:5:"line3";s:0:"";s:11:"postal_code";s:5:"63141";s:10:"state_code";s:2:"MO";}}', ), ), 5 => array ( 'value' => array ( 'lists' => 'a:1:{i:0;a:1:{s:2:"id";s:10:"1966704516";}}', ), ), ), ));
    }

    public function getTestData() {
        $submission = new stdClass();
        return (object)(array( 'nid' => '28', 'uid' => '1', 'submitted' => 1405093809, 'remote_addr' => '64.90.21.130', 'is_draft' => 0, 'data' => array ( 2 => array ( 'value' => array ( 'addresses' => array ( 0 => array ( 'address_type' => '0', 'city' => 'saint louis', 'country_code' => 'US', 'line1' => '401 n lindberg', 'line2' => '', 'line3' => '', 'postal_code' => '63141', 'state_code' => 'MO', ), ), ), ), 3 => array ( 'value' => array ( 'first_name' => 'jhj', ), ), ), ));
    }
}
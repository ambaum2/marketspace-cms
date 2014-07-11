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
    public function testGetSubmission() {
        $submission = $this->getTestData();
        $node = node_load($submission->nid);
        $cc = new MS_Constant_Contact_Contacts_Submission($submission, $node);
        $result = $cc->GetSubmissionJson();
        $this->assertEquals(2, count($result));
        print_r($result);
    }

    public function getTestData() {
        $submission = new stdClass();
        return (object)(array( 'nid' => '28', 'uid' => '1', 'submitted' => 1405093809, 'remote_addr' => '64.90.21.130', 'is_draft' => 0, 'data' => array ( 2 => array ( 'value' => array ( 'addresses' => array ( 0 => array ( 'address_type' => '0', 'city' => 'saint louis', 'country_code' => 'US', 'line1' => '401 n lindberg', 'line2' => '', 'line3' => '', 'postal_code' => '63141', 'state_code' => 'MO', ), ), ), ), 3 => array ( 'value' => array ( 'first_name' => 'jhj', ), ), ), ));
    }
}
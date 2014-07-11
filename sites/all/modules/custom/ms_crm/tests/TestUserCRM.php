<?php
require 'PHPUnit/Autoload.php';
class TestUserCRM extends PHPUnit_Framework_TestCase
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
    public function testSendEmail() {


    }
}
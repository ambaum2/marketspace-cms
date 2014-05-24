<?php
require 'PHPUnit/Autoload.php';
class MS_Render_ReportsTests extends PHPUnit_Framework_TestCase
{
    public $dpl_dir = '/var/www/mspacecms/';
    const DRUPAL_ROOT = '/var/www/mspacecms/';
    const REMOTE_ADDR = '162.243.110.111';

    public function __construct() {
        error_reporting(E_ALL);
        ini_set("display_errors", 1);
        $_SERVER['HTTP_HOST'] = basename($this->dpl_dir);
        $_SERVER['REMOTE_ADDR'] = '127.0.0.1';
        define('DRUPAL_ROOT', $this->dpl_dir);
        set_include_path($this->dpl_dir . PATH_SEPARATOR . get_include_path());
        require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
        drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    }
    public function testIntegrationGetReports() {
        $Api = new MS_Api();
        $report = $Api->apiTokenCall("/ms-api/reports/owners/2/orders/types/ProductOrders/39", false);
        print_r($report);
    }
}
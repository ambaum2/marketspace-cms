<?php
require 'PHPUnit/Autoload.php';
class es_webform_magento_product_attributeTest extends PHPUnit_Framework_TestCase
{
	public $dpl_dir = '/var/www/mspacecms/';
	const DRUPAL_ROOT = '/var/www/mspacecms/';
	const REMOTE_ADDR = '162.243.110.111';
	
	public function __construct() {
		$_SERVER['HTTP_HOST'] = basename($this->dpl_dir);
		$_SERVER['REMOTE_ADDR'] = '127.0.0.1';	
		define('DRUPAL_ROOT', $this->dpl_dir);
		set_include_path($this->dpl_dir . PATH_SEPARATOR . get_include_path());
		require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);		
	}
	
  public function testattribute_exists() {
			$magento_attribute = new magento_product_attributes();
			$result = $magento_attribute->exists_magento_attribute(64);
			$this->assertTrue($result);
		}
	public function testget_stored_magento_attribute_data_by_code() {
		$m = new magento_product_attributes();
		$result = $m->get_stored_magento_attribute_data_by_code('price');

		$this->assertEquals('Price', $result->frontend_label);
		$this->assertTrue(is_object($result));
	}
	
  }
<?php
require 'PHPUnit/Autoload.php';
class productTest extends PHPUnit_Framework_TestCase
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
  public function testcreate_attribute_input() {
    $p = new product;
    $attributes = $this->getproductAttributeTestData();
    $components = $this->getProductComponentTestData();
    $testData = $this->getProductAttributeCreateTestData();
    $i = 0;
    foreach($attributes as $key=>$value) {
      $result = $p->create_attribute_input((object)$value, null, $components[$i]);
      $this->assertEquals($testData[$i], $result);
      $i++;
    }
  }
  public function getAttributeOptionTestData() {
    return array ( '' => '-- Please Select --', 0 => 'None', 2 => 'Taxable Goods', 4 => 'Shipping', );
  }
  public function getProductAttributeCreateTestData() {
    $form = array();
    $form[] = array (
  '#type' => 'textfield',
  '#title' => 'Name',
  '#required' => true,
  '#description' => '',
);
    $form[] = array (
  '#type' => 'textfield',
  '#title' => 'Price',
  '#required' => true,
  '#description' => '',
);
    $form[] = array (
  '#type' => 'textfield',
  '#title' => 'Special Price',
  '#required' => false,
  '#description' => '',
);
    $form[] = array (
  '#type' => 'date',
  '#title' => 'Special Price From Date',
  '#required' => false,
  '#description' => '',
);
    $form[] = array (
  '#type' => 'date',
  '#title' => 'Special Price To Date',
  '#required' => false,
  '#description' => '',
);
    $form[] = array (
  '#type' => 'select',
  '#title' => 'Tax Class',
  '#required' => true,
  '#description' => '',
  '#options' =>
  array (
    '' => '-- Please Select --',
    0 => 'None',
    2 => 'Taxable Goods',
    4 => 'Shipping',
  ),
);
    $form[] = array (
  'text' =>
  array (
    '#type' => 'fieldset',
    '#title' => 'Editor',
    '#weight' => 5,
    '#collapsible' => true,
    '#collapsed' => false,
    'textarea' =>
    array (
      '#type' => 'text_format',
      '#format' => 'content_admin',
      '#title' => 'Description',
      '#required' => true,
      '#description' => '',
      0 =>
      array (
        '#attached' =>
        array (
          'css' =>
          array (
            0 => 'sites/all/modules/custom/es_webform/css/es_webform_wysiwyg.css',
          ),
        ),
      ),
    ),
  ),
);
    $form[] = array (
  'text' =>
  array (
    '#type' => 'fieldset',
    '#title' => 'Editor',
    '#weight' => 5,
    '#collapsible' => true,
    '#collapsed' => false,
    'textarea' =>
    array (
      '#type' => 'text_format',
      '#format' => 'content_admin',
      '#title' => 'Short Description',
      '#required' => true,
      '#description' => '',
      0 =>
      array (
        '#attached' =>
        array (
          'css' =>
          array (
            0 => 'sites/all/modules/custom/es_webform/css/es_webform_wysiwyg.css',
          ),
        ),
      ),
    ),
  ),
);
  return $form;
  }
  /**
   *
   * @return array returns test attribute data
   */
  public function getproductAttributeTestData() {
    $attributesContainer = array();
    $attributesContainer[] = array( 'attribute_id' => '71', 'entity_type_id' => '4', 'attribute_code' => array ( 'code' => 'name', ), 'attribute_model' => NULL, 'backend_model' => NULL, 'backend_type' => 'varchar', 'backend_table' => NULL, 'frontend_model' => NULL, 'frontend_input' => 'text', 'frontend_label' => 'Name', 'frontend_class' => NULL, 'source_model' => NULL, 'is_required' => '1', 'is_user_defined' => '0', 'default_value' => NULL, 'is_unique' => '0', 'note' => NULL, 'frontend_input_renderer' => NULL, 'is_global' => '0', 'is_visible' => '1', 'is_searchable' => '1', 'is_filterable' => '0', 'is_comparable' => '0', 'is_visible_on_front' => '0', 'is_html_allowed_on_front' => '0', 'is_used_for_price_rules' => '0', 'is_filterable_in_search' => '0', 'used_in_product_listing' => '1', 'used_for_sort_by' => '1', 'is_configurable' => '1', 'apply_to' => NULL, 'is_visible_in_advanced_search' => '1', 'position' => '0', 'is_wysiwyg_enabled' => '0', 'is_used_for_promo_rules' => '0', );
    $attributesContainer[] = array( 'attribute_id' => '75', 'entity_type_id' => '4', 'attribute_code' => array ( 'code' => 'price', ), 'attribute_model' => NULL, 'backend_model' => 'catalog/product_attribute_backend_price', 'backend_type' => 'decimal', 'backend_table' => NULL, 'frontend_model' => NULL, 'frontend_input' => 'price', 'frontend_label' => 'Price', 'frontend_class' => NULL, 'source_model' => NULL, 'is_required' => '1', 'is_user_defined' => '0', 'default_value' => NULL, 'is_unique' => '0', 'note' => NULL, 'frontend_input_renderer' => NULL, 'is_global' => '2', 'is_visible' => '1', 'is_searchable' => '1', 'is_filterable' => '1', 'is_comparable' => '0', 'is_visible_on_front' => '0', 'is_html_allowed_on_front' => '0', 'is_used_for_price_rules' => '0', 'is_filterable_in_search' => '0', 'used_in_product_listing' => '1', 'used_for_sort_by' => '1', 'is_configurable' => '1', 'apply_to' => 'simple,configurable,virtual,bundle,downloadable', 'is_visible_in_advanced_search' => '1', 'position' => '0', 'is_wysiwyg_enabled' => '0', 'is_used_for_promo_rules' => '0', );
    $attributesContainer[] = array( 'attribute_id' => '76', 'entity_type_id' => '4', 'attribute_code' => array ( 'code' => 'special_price', ), 'attribute_model' => NULL, 'backend_model' => 'catalog/product_attribute_backend_price', 'backend_type' => 'decimal', 'backend_table' => NULL, 'frontend_model' => NULL, 'frontend_input' => 'price', 'frontend_label' => 'Special Price', 'frontend_class' => NULL, 'source_model' => NULL, 'is_required' => '0', 'is_user_defined' => '0', 'default_value' => NULL, 'is_unique' => '0', 'note' => NULL, 'frontend_input_renderer' => NULL, 'is_global' => '2', 'is_visible' => '1', 'is_searchable' => '0', 'is_filterable' => '0', 'is_comparable' => '0', 'is_visible_on_front' => '0', 'is_html_allowed_on_front' => '0', 'is_used_for_price_rules' => '0', 'is_filterable_in_search' => '0', 'used_in_product_listing' => '1', 'used_for_sort_by' => '0', 'is_configurable' => '1', 'apply_to' => 'simple,configurable,virtual,bundle,downloadable', 'is_visible_in_advanced_search' => '0', 'position' => '0', 'is_wysiwyg_enabled' => '0', 'is_used_for_promo_rules' => '0', );
    $attributesContainer[] = array( 'attribute_id' => '77', 'entity_type_id' => '4', 'attribute_code' => array ( 'code' => 'special_from_date', ), 'attribute_model' => NULL, 'backend_model' => 'catalog/product_attribute_backend_startdate', 'backend_type' => 'datetime', 'backend_table' => NULL, 'frontend_model' => NULL, 'frontend_input' => 'date', 'frontend_label' => 'Special Price From Date', 'frontend_class' => NULL, 'source_model' => NULL, 'is_required' => '0', 'is_user_defined' => '0', 'default_value' => NULL, 'is_unique' => '0', 'note' => NULL, 'frontend_input_renderer' => NULL, 'is_global' => '2', 'is_visible' => '1', 'is_searchable' => '0', 'is_filterable' => '0', 'is_comparable' => '0', 'is_visible_on_front' => '0', 'is_html_allowed_on_front' => '0', 'is_used_for_price_rules' => '0', 'is_filterable_in_search' => '0', 'used_in_product_listing' => '1', 'used_for_sort_by' => '0', 'is_configurable' => '1', 'apply_to' => 'simple,configurable,virtual,bundle,downloadable', 'is_visible_in_advanced_search' => '0', 'position' => '0', 'is_wysiwyg_enabled' => '0', 'is_used_for_promo_rules' => '0', );
    $attributesContainer[] = array( 'attribute_id' => '78', 'entity_type_id' => '4', 'attribute_code' => array ( 'code' => 'special_to_date', ), 'attribute_model' => NULL, 'backend_model' => 'eav/entity_attribute_backend_datetime', 'backend_type' => 'datetime', 'backend_table' => NULL, 'frontend_model' => NULL, 'frontend_input' => 'date', 'frontend_label' => 'Special Price To Date', 'frontend_class' => NULL, 'source_model' => NULL, 'is_required' => '0', 'is_user_defined' => '0', 'default_value' => NULL, 'is_unique' => '0', 'note' => NULL, 'frontend_input_renderer' => NULL, 'is_global' => '2', 'is_visible' => '1', 'is_searchable' => '0', 'is_filterable' => '0', 'is_comparable' => '0', 'is_visible_on_front' => '0', 'is_html_allowed_on_front' => '0', 'is_used_for_price_rules' => '0', 'is_filterable_in_search' => '0', 'used_in_product_listing' => '1', 'used_for_sort_by' => '0', 'is_configurable' => '1', 'apply_to' => 'simple,configurable,virtual,bundle,downloadable', 'is_visible_in_advanced_search' => '0', 'position' => '0', 'is_wysiwyg_enabled' => '0', 'is_used_for_promo_rules' => '0', );
    $attributesContainer[] = array( 'attribute_id' => '122', 'entity_type_id' => '4', 'attribute_code' => array ( 'code' => 'tax_class_id', ), 'attribute_model' => NULL, 'backend_model' => NULL, 'backend_type' => 'int', 'backend_table' => NULL, 'frontend_model' => NULL, 'frontend_input' => 'select', 'frontend_label' => 'Tax Class', 'frontend_class' => NULL, 'source_model' => 'tax/class_source_product', 'is_required' => '1', 'is_user_defined' => '0', 'default_value' => NULL, 'is_unique' => '0', 'note' => NULL, 'frontend_input_renderer' => NULL, 'is_global' => '2', 'is_visible' => '1', 'is_searchable' => '1', 'is_filterable' => '0', 'is_comparable' => '0', 'is_visible_on_front' => '0', 'is_html_allowed_on_front' => '0', 'is_used_for_price_rules' => '0', 'is_filterable_in_search' => '0', 'used_in_product_listing' => '1', 'used_for_sort_by' => '0', 'is_configurable' => '1', 'apply_to' => 'simple,configurable,virtual,downloadable,bundle', 'is_visible_in_advanced_search' => '1', 'position' => '0', 'is_wysiwyg_enabled' => '0', 'is_used_for_promo_rules' => '0', 'options' => array ( '' => '-- Please Select --', 0 => 'None', 2 => 'Taxable Goods', 4 => 'Shipping', ), );
    $attributesContainer[] = array( 'attribute_id' => '72', 'entity_type_id' => '4', 'attribute_code' => array ( 'code' => 'description', ), 'attribute_model' => NULL, 'backend_model' => NULL, 'backend_type' => 'text', 'backend_table' => NULL, 'frontend_model' => NULL, 'frontend_input' => 'textarea', 'frontend_label' => 'Description', 'frontend_class' => NULL, 'source_model' => NULL, 'is_required' => '1', 'is_user_defined' => '0', 'default_value' => NULL, 'is_unique' => '0', 'note' => NULL, 'frontend_input_renderer' => NULL, 'is_global' => '0', 'is_visible' => '1', 'is_searchable' => '1', 'is_filterable' => '0', 'is_comparable' => '1', 'is_visible_on_front' => '0', 'is_html_allowed_on_front' => '1', 'is_used_for_price_rules' => '0', 'is_filterable_in_search' => '0', 'used_in_product_listing' => '0', 'used_for_sort_by' => '0', 'is_configurable' => '1', 'apply_to' => NULL, 'is_visible_in_advanced_search' => '1', 'position' => '0', 'is_wysiwyg_enabled' => '1', 'is_used_for_promo_rules' => '0', );
    $attributesContainer[] = array( 'attribute_id' => '73', 'entity_type_id' => '4', 'attribute_code' => array ( 'code' => 'short_description', ), 'attribute_model' => NULL, 'backend_model' => NULL, 'backend_type' => 'text', 'backend_table' => NULL, 'frontend_model' => NULL, 'frontend_input' => 'textarea', 'frontend_label' => 'Short Description', 'frontend_class' => NULL, 'source_model' => NULL, 'is_required' => '1', 'is_user_defined' => '0', 'default_value' => NULL, 'is_unique' => '0', 'note' => NULL, 'frontend_input_renderer' => NULL, 'is_global' => '0', 'is_visible' => '1', 'is_searchable' => '1', 'is_filterable' => '0', 'is_comparable' => '1', 'is_visible_on_front' => '0', 'is_html_allowed_on_front' => '1', 'is_used_for_price_rules' => '0', 'is_filterable_in_search' => '0', 'used_in_product_listing' => '1', 'used_for_sort_by' => '0', 'is_configurable' => '1', 'apply_to' => NULL, 'is_visible_in_advanced_search' => '1', 'position' => '0', 'is_wysiwyg_enabled' => '1', 'is_used_for_promo_rules' => '0', ); 
    
      
    return $attributesContainer;
  }
  public function getProductComponentTestData() {
    $attributesContainer = array();
    $attributesContainer[] = array ( 'nid' => 11, 'cid' => '2', 'pid' => '3', 'form_key' => 'name', 'name' => 'Name', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'name', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '11', 'page_num' => 1, );
    $attributesContainer[] = array ( 'nid' => 11, 'cid' => '1', 'pid' => '11', 'form_key' => 'price', 'name' => 'Price', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'price', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '12', 'page_num' => 1, );
    $attributesContainer[] = array ( 'nid' => 11, 'cid' => '12', 'pid' => '11', 'form_key' => 'special_price', 'name' => 'Special Price', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'special_price', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '13', 'page_num' => 1, );
    $attributesContainer[] = array ( 'nid' => 11, 'cid' => '13', 'pid' => '11', 'form_key' => 'special_start_date', 'name' => 'Special Start Date', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'special_from_date', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '14', 'page_num' => 1, );
    $attributesContainer[] = array ( 'nid' => 11, 'cid' => '14', 'pid' => '11', 'form_key' => 'special_end_date', 'name' => 'Special End Date', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'special_to_date', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '15', 'page_num' => 1, );
    $attributesContainer[] = array ( 'nid' => 11, 'cid' => '15', 'pid' => '11', 'form_key' => 'tax_class', 'name' => 'Tax Class', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'tax_class_id', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '16', 'page_num' => 1, );
    $attributesContainer[] = array ( 'nid' => 11, 'cid' => '5', 'pid' => '4', 'form_key' => 'description', 'name' => 'Description', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'description', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '13', 'page_num' => 1, );
    $attributesContainer[] = array ( 'nid' => 11, 'cid' => '6', 'pid' => '4', 'form_key' => 'short_description', 'name' => 'Short Description', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'short_description', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '20', 'page_num' => 1, );
    return $attributesContainer;
  }

}
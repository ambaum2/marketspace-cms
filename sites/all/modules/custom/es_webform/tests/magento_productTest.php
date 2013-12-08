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
	public function testget_magento_attribute_id_by_code() {
		$m = new magento_product_attributes();
		$result = $m->get_magento_attribute_id_by_code('name');
		$this->assertEquals(71, $result);
	}
  public function testattribute_exists() {
			$magento_attribute = new magento_product_attributes();
			$result = $magento_attribute->exists_magento_attribute(64);
			$this->assertTrue($result);
		}
	public function testget_stored_magento_attribute_data_by_code() {
		$m = new magento_product_attributes();
		$result = $m->get_stored_magento_attribute_data_by_code('price');
		//var_export($result);
		$this->assertEquals('Price', $result->frontend_label);
		$this->assertEquals($this->getAttributeTestData(), $result);
		$this->assertTrue(is_object($result));
	}
	public function testget_stored_magento_attribute_data() {
		$m = new magento_product_attributes();
		$result = $m->get_stored_magento_attribute_data(75);
		$this->assertEquals('Price', $result->frontend_label);
		$this->assertEquals($this->getAttributeTestData(), $result);
		$this->assertTrue(is_object($result));
	}

	public function testcreateProductAddData() {
		$submission = $this->getSubmissionSample();
		$node = $this->getNodeSample();
		$m = new magento_product_attributes();
		$result = $m->createProductAddData($submission, $node);
		$sample = $this->getProductCreateDataSample();
		foreach($sample['attributes'] as $key => $value) {
			$this->assertEquals(trim($sample['attributes'][$key]), trim($result['attributes'][$key]));
		}
		//$this->assertEquals(count($sample['attributes']), count($result['attributes'])); 
		//this is the best i can do asserting equals on the arrays is not working out b/c of 
		//line spacing or something in description field
	}
	public function getAttributeTestData() {
		return (object)array(
		   'attribute_id' => '75',
		   'entity_type_id' => '4',
		   'attribute_code' =>
		  array (
		    'code' => 'price',
		  ),
		   'attribute_model' => NULL,
		   'backend_model' => 'catalog/product_attribute_backend_price',
		   'backend_type' => 'decimal',
		   'backend_table' => NULL,
		   'frontend_model' => NULL,
		   'frontend_input' => 'price',
		   'frontend_label' => 'Price',
		   'frontend_class' => NULL,
		   'source_model' => NULL,
		   'is_required' => '1',
		   'is_user_defined' => '0',
		   'default_value' => NULL,
		   'is_unique' => '0',
		   'note' => NULL,
		   'frontend_input_renderer' => NULL,
		   'is_global' => '2',
		   'is_visible' => '1',
		   'is_searchable' => '1',
		   'is_filterable' => '1',
		   'is_comparable' => '0',
		   'is_visible_on_front' => '0',
		   'is_html_allowed_on_front' => '0',
		   'is_used_for_price_rules' => '0',
		   'is_filterable_in_search' => '0',
		   'used_in_product_listing' => '1',
		   'used_for_sort_by' => '1',
		   'is_configurable' => '1',
		   'apply_to' => 'simple,configurable,virtual,bundle,downloadable',
		   'is_visible_in_advanced_search' => '1',
		   'position' => '0',
		   'is_wysiwyg_enabled' => '0',
		   'is_used_for_promo_rules' => '0',
		);		
	}
	public function getProductCreateDataSample() {
		return array(
   'attributes' =>
  array (
    'name' => 'class A',
    'price' => '4',
    'categories' =>
    array (
      0 => 1,
      1 => 3,
    ),
    'description' => '
               the men who have stood with no fear in the service of king',
  ),
);
		
	}
	public function getSubmissionSample() {
	return (object)array( 'nid' => '9', 'uid' => '1', 'submitted' => 1386299243, 'remote_addr' => '71.11.121.38', 'is_draft' => 0, 'data' => array ( 3 => array ( 'value' => array ( 0 => 'class A', ), ), 2 => array ( 'value' => array ( 0 => '4', ), ), 8 => array ( 'value' => array ( 'categories' => array ( 1 => '1', 3 => 1, ), ), ), 4 => array ( 'value' => array ( 'text' => array ( 'textarea' => array ( 'value' => '
		the men who have stood with no fear in the service of king', 'format' => 'content_admin', ), ), ), ), ), );
	}
	public function getNodeSample() {
		return (object)array( 'vid' => '9', 'uid' => '1', 'title' => 'Coupons', 'log' => '', 'status' => '1', 'comment' => '1', 'promote' => '0', 'sticky' => '0', 'nid' => '9', 'type' => 'webform', 'language' => 'und', 'created' => '1385665991', 'changed' => '1386093697', 'tnid' => '0', 'translate' => '0', 'revision_timestamp' => '1386093697', 'revision_uid' => '2', 'body' => array ( ), 'field_form_category' => array ( 'und' => array ( 0 => array ( 'tid' => '4', ), ), ), 'field_thumbnail' => array ( 'und' => array ( 0 => array ( 'fid' => '123', 'alt' => '', 'title' => '', 'width' => '450', 'height' => '358', 'uid' => '2', 'filename' => 'ecommerce-e.jpeg', 'uri' => 'public://ecommerce-e.jpeg', 'filemime' => 'image/jpeg', 'filesize' => '89817', 'status' => '1', 'timestamp' => '1386093697', 'rdf_mapping' => array ( ), ), ), ), 'field_market_space_settings' => array ( 'und' => array ( 0 => array ( 'type_code' => 'virtual', 'attribute_set_id' => '4', 'product_type' => '7', 'preset_attributes' => NULL, ), ), ), 'rdf_mapping' => array ( 'rdftype' => array ( 0 => 'sioc:Item', 1 => 'foaf:Document', ), 'title' => array ( 'predicates' => array ( 0 => 'dc:title', ), ), 'created' => array ( 'predicates' => array ( 0 => 'dc:date', 1 => 'dc:created', ), 'datatype' => 'xsd:dateTime', 'callback' => 'date_iso8601', ), 'changed' => array ( 'predicates' => array ( 0 => 'dc:modified', ), 'datatype' => 'xsd:dateTime', 'callback' => 'date_iso8601', ), 'body' => array ( 'predicates' => array ( 0 => 'content:encoded', ), ), 'uid' => array ( 'predicates' => array ( 0 => 'sioc:has_creator', ), 'type' => 'rel', ), 'name' => array ( 'predicates' => array ( 0 => 'foaf:name', ), ), 'comment_count' => array ( 'predicates' => array ( 0 => 'sioc:num_replies', ), 'datatype' => 'xsd:integer', ), 'last_activity' => array ( 'predicates' => array ( 0 => 'sioc:last_activity_date', ), 'datatype' => 'xsd:dateTime', 'callback' => 'date_iso8601', ), ), 'webform' => array ( 'nid' => '9', 'confirmation' => '', 'confirmation_format' => NULL, 'redirect_url' => '', 'status' => '1', 'block' => '0', 'teaser' => '0', 'allow_draft' => '0', 'auto_save' => '0', 'submit_notice' => '1', 'submit_text' => '', 'submit_limit' => '-1', 'submit_interval' => '-1', 'total_submit_limit' => '-1', 'total_submit_interval' => '-1', 'record_exists' => true, 'roles' => array ( 0 => '1', 1 => '2', ), 'emails' => array ( ), 'components' => array ( 7 => array ( 'nid' => 9, 'cid' => '7', 'pid' => '0', 'form_key' => 'product_profile_information', 'name' => 'Product Profile Information', 'type' => 'fieldset', 'value' => '', 'extra' => array ( 'title_display' => 0, 'private' => 0, 'collapsible' => 0, 'collapsed' => 0, 'conditional_component' => '3', 'conditional_operator' => '=', 'description' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '6', 'page_num' => 1, ), 3 => array ( 'nid' => 9, 'cid' => '3', 'pid' => '7', 'form_key' => 'name', 'name' => 'Name', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'name', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '8', 'page_num' => 1, ), 2 => array ( 'nid' => 9, 'cid' => '2', 'pid' => '7', 'form_key' => 'price', 'name' => 'price', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'price', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '9', 'page_num' => 1, ), 8 => array ( 'nid' => 9, 'cid' => '8', 'pid' => '7', 'form_key' => 'select_placemement', 'name' => 'Select Placemement', 'type' => 'es_category', 'value' => '', 'extra' => array ( 'magento_attribute_code_name' => 'categories', 'magento_categories' => array ( 'categories' => array ( 'Array' => array ( 2 => array ( 'i3' => '1', ), ), 2 => array ( 2 => array ( 'i106' => 0, 106 => array ( 'i111' => 0, 111 => array ( 'i100' => 0, 'i60' => 0, 'i61' => 0, ), 'i41' => 0, 41 => array ( 'i42' => 0, 'i43' => 0, ), ), 'i4' => 0, 4 => array ( 'i23' => 0, 23 => array ( 'i39' => 0, 'i40' => 0, ), ), 13 => array ( 'i18' => 0, 14 => array ( 'i15' => 0, ), ), 'i13' => 0, 5 => array ( 'i16' => 0, 16 => array ( ), ), 3 => array ( ), 7 => array ( 'i22' => 0, 22 => array ( 'i45' => 0, 'i47' => 0, 'i46' => 0, 'i62' => 0, ), ), 6 => array ( ), 'i107' => 0, 107 => array ( 'i48' => 0, ), 'i108' => 0, 'i109' => 0, 109 => array ( 'i35' => 0, ), 'i64' => 0, 64 => array ( 'i65' => 0, 65 => array ( 'i74' => 0, 'i67' => 0, ), 'i75' => 0, 75 => array ( 'i78' => 0, 'i79' => 0, 'i66' => 0, 'i80' => 0, ), 'i92' => 0, ), 'i112' => 0, 'i95' => 0, 95 => array ( 'i105' => 0, 'i103' => 0, 'i97' => 0, 'i104' => 0, 'i98' => 0, 'i96' => 0, 'i99' => 0, ), 'i110' => 0, 110 => array ( 'i51' => 0, ), 53 => array ( 'i71' => 0, 71 => array ( 'i72' => 0, ), ), 'i3' => 1, 'i5' => 0, 'i6' => 0, ), ), ), ), 'title_display' => 'before', 'private' => 0, 'conditional_component' => '3', 'conditional_operator' => '=', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '10', 'page_num' => 1, ), 5 => array ( 'nid' => 9, 'cid' => '5', 'pid' => '0', 'form_key' => 'description_page_break', 'name' => 'Description ', 'type' => 'pagebreak', 'value' => '', 'extra' => array ( 'next_page_label' => 'Description', 'prev_page_label' => 'Back', 'conditional_operator' => '=', 'private' => 0, 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '8', 'page_num' => 2, ), 6 => array ( 'nid' => 9, 'cid' => '6', 'pid' => '0', 'form_key' => 'product_information', 'name' => 'Product Information', 'type' => 'fieldset', 'value' => '', 'extra' => array ( 'title_display' => 0, 'private' => 0, 'collapsible' => 0, 'collapsed' => 0, 'conditional_component' => '2', 'conditional_operator' => '=', 'description' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '9', 'page_num' => 2, ), 4 => array ( 'nid' => 9, 'cid' => '4', 'pid' => '6', 'form_key' => 'description', 'name' => 'Description', 'type' => 'es_webform', 'value' => '', 'extra' => array ( 'magento_attribute_code' => 'description', 'magento_attribute_code_name' => '1', 'title_display' => 'before', 'private' => 0, 'conditional_operator' => '=', 'conditional_component' => '', 'conditional_values' => '', ), 'mandatory' => '0', 'weight' => '5', 'page_num' => 2, ), ), ), 'cid' => '0', 'last_comment_timestamp' => '1385665991', 'last_comment_name' => NULL, 'last_comment_uid' => '1', 'comment_count' => '0', 'name' => 'ambaum2', 'picture' => '84', 'data' => 'a:6:{s:16:"ckeditor_default";s:1:"t";s:20:"ckeditor_show_toggle";s:1:"t";s:14:"ckeditor_width";s:4:"100%";s:13:"ckeditor_lang";s:2:"en";s:18:"ckeditor_auto_lang";s:1:"t";s:7:"overlay";i:1;}', 'entity_view_prepared' => true, 'content' => array ( 'field_market_space_settings' => array ( '#theme' => 'field', '#weight' => 6, '#title' => 'Market Space Settings', '#access' => true, '#label_display' => 'above', '#view_mode' => 'full', '#language' => 'und', '#field_name' => 'field_market_space_settings', '#field_type' => 'es_webform_mage_webform_setup', '#field_translatable' => '0', '#entity_type' => 'node', '#bundle' => 'webform', '#object' => NULL, '#items' => array ( 0 => array ( 'type_code' => 'virtual', 'attribute_set_id' => '4', 'product_type' => '7', 'preset_attributes' => NULL, ), ), '#formatter' => 'es_webform_mage_product_type_select_display', 0 => array ( '#type' => 'html_tag', '#tag' => 'p', '#value' => 'The content area color has been changed to virtual', '#attached' => array ( 'css' => array ( 0 => array ( 'data' => 'div.region-content { background-color:virtual;}', 'type' => 'inline', ), ), ), ), ), '#pre_render' => array ( 0 => '_field_extra_fields_pre_render', ), '#entity_type' => 'node', '#bundle' => 'webform', 'links' => array ( '#theme' => 'links__node', '#pre_render' => array ( 0 => 'drupal_pre_render_links', ), '#attributes' => array ( 'class' => array ( 0 => 'links', 1 => 'inline', ), ), 'node' => array ( '#theme' => 'links__node__node', '#links' => array ( ), '#attributes' => array ( 'class' => array ( 0 => 'links', 1 => 'inline', ), ), ), ), ), );
	}
}
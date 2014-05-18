<?php

class MS_Render {
	public $Uid;

    /**
     * @return array
     */
    public function renderProductGrid() {
        $api = new MS_Api();
        $products = $api->apiTokenCall("/ms-api/product/users/{$this->Uid}/lists/ProductType", false);
		return $products;
	}
  /*
   * edit page for products
   */
  public function renderProductEdit() {
    
    //return $product;
  }  
  
  /*
   * get nodes by tid vs
   *  field_data_field_form_category
   */
  public function getWebformsByTidAndFormCategory($tid) {
    $query = db_select('field_data_field_form_category', 'a')
      ->fields('a',array('entity_id'))
      ->condition('field_form_category_tid', (int)$tid);
    $node_webforms = $query->execute()->fetchAll();
    if(count($node_webforms) > 0) {
      return $node_webforms;
    } else {
      return false;
    }
  }
}


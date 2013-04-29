<?php

class marketSpaceDashboard {
	
	public function renderProductGrid() {
		$magento = new magento_product;
    global $user;

		$complexFilter = array(
	    'complex_filter' => array(
	        array(
	            'key' => 'marketspace_owner',
	            'value' => array('key' => 'in', 'value' => $user->uid)
	        )
	    )
		);
		$products = $magento->getAllUsersProducts($complexFilter);
		return $products;
	}
  /*
   * edit page for products
   */
  public function renderProductEdit() {
    
    //return $product;
  }  
}


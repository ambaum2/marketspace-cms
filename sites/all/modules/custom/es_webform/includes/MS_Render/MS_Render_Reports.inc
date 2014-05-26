<?php
class MS_Render_Reports implements iMS_Render {
    public $Uid;
    public $Api;
    public $Products;
    public $Method;
    /** @var $ListType String */
    public $ListType;
    /** @var $ListTypeParameters String */
    public $ListTypeParameters;
    function __construct() {
        if(!isset($Api)) {
            $this->Api = new MS_Api();
        }
    }
    public function render()
    {
        try {
            $Method = 'Render' . $this->Method;
            $result = $this->{$Method}();
        } catch(Exception $e) {

        }
        return $result;
    }

    /**
     * @return array
     */
    public function RenderReportsGrid() {
        try {
            if(empty($this->Uid))
                throw new Exception("Uid not set");

            $products = $this->Api->apiTokenCall("/ms-api/reports/owners/{$this->Uid}/orders/types/{$this->ListType}/{$this->ListTypeParameters}", false);
        } catch(Exception $e) {
            throw new Exception($e->getMessage());
        }
        return $products;
    }

    /**
     * @return array
     */
    public function RenderProductChart() {
        try {
            if(empty($this->Uid))
                throw new Exception("Uid not set");
            if(empty($this->Products))
                throw new Exception("Product(s) not set");

            $products = $this->Api->apiTokenCall("/ms-api/reports/owners/{$this->Uid}/orders/types/ProductOrders/{$this->Products}", false);
        } catch(Exception $e) {
            throw new Exception($e->getMessage());
        }
        return $products;
    }
}
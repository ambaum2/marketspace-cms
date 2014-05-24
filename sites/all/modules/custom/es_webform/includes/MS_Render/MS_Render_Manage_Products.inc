<?php
class MS_Render_Manage_Products implements iMS_Render {
    public $Uid;
    public $Api;
    public $Method;
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
    public function RenderProductGrid() {
        try {
            if(empty($this->Uid))
                throw new Exception("Uid not set");

            $products = $this->Api->apiTokenCall("/ms-api/product/users/{$this->Uid}/lists/ProductType", false);
        } catch(Exception $e) {
            throw new Exception($e->getMessage());
        }
        return $products;
    }
}
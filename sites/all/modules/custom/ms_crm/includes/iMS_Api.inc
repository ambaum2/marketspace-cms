<?php

interface iMS_Api {
    public function request($path, $object = true); /** make the request */
    public function PostRequest($path, $object = true);
    public function setParameters();
}
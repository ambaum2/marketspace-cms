<?php

/**
 * Class MS_Individuals_Contacts
 * this class deals with collections of
 * contacts. See MS_Individuals_Contacts_Item
 * for handling a single contact
 */
class MS_Individuals_Contacts implements iMS_CRM {
    private $contactsRepository; /** iMS_Api $contactsRepository  */
    private $queryString;
    private $uri;
    public $postBody;
    private $post_responses = array( //@todo put these in a database table if needed
        '201' => 'success',
        '400' => 'bad data',
        '401' => 'authentication error contact administrator',
        '409' => 'email address %email_address already exists',
        '500' => 'internal server error please try again',
    );
    public function __construct(iMS_Api $contactsRepository, $uri = null, $params = null) {
        if(!isset($contactsRepository)) {
            $this->contactsRepository = new MS_CRM_Api();
        } else {
            $this->contactsRepository = $contactsRepository;
        }
        $this->contactsRepository->parameters = $params;
        $this->queryString = $this->contactsRepository->setParameters();

        if(isset($uri)) {
            $this->uri = $uri;
        } else {
            $this->uri = "contacts"; //@TODO set some default maybe should throw error?
        }
    }
    public function get()
    {
        $result = $this->contactsRepository->request($this->uri . $this->queryString);
        return $result;
    }

    /** get an entity or entities
     *  https://api.constantcontact.com/v2/contacts?action_by=ACTION_BY_OWNER&api_key=6x3qh7weh2cthgyqgm8vy8zp
     */
    public function post()
    {
        if(isset($this->postBody)) { //@TODO should this be here
            $this->contactsRepository->postBody = json_encode($this->postBody);
        } else {
            throw new Exception('No data for adding a contact sent');
        }
        try {
        $result = $this->contactsRepository->PostRequest($this->uri . $this->queryString);
        } catch(Exception $e) {
            watchdog('ms_crm', 'create a contact failed <pre>%post_data</pre>', array('%post_data' => print_r($this->postBody), WATCHDOG_ERROR));
            throw new Exception($e->getMessage());
        }
        $result = $this->GetErrorMessage('post', $result);
        return $result;
    }

    /** create an entity */
    public function put()
    {
        // TODO: Implement put() method.
    }

    /** create a batch of entities */
    public function delete()
    {
        // TODO: Implement delete() method.
    }

    /** delete an entity */
    public function patch()
    {
        // TODO: Implement patch() method.
    }

    /**
     * gets the primary address by error code
     * constant contact gives back pretty good errors so I'm
     * leaning towards removing this unless we find
     * somethime we don't like
     * @todo decide whether to improve or remove this entirely after testing - this has flaws for ex: only accounts for one email address that could possibly be okay if their were multiples
     * @param string $type
     * @param $result
     * @return mixed
     */
    public function GetErrorMessage($type = 'get', $result) {
        if(isset($result['error'])) {
            if(array_key_exists($result['error']['code'], $this->post_responses)) {
                $result['primary_error_message'] = t($this->post_responses[$result['error']['code']], array('%email_address' => $this->postBody['email_addresses'][0]['email_address']));
            } else {
                $result['primary_error_message'] = 'an error occurred please try again';
            }
        }
        return $result;
    }

    /**
     * @return mixed
     */
    public function getQueryString()
    {
        return $this->queryString;
    }

    /**
     * @return string
     */
    public function getUri()
    {
        return $this->uri;
    }

}
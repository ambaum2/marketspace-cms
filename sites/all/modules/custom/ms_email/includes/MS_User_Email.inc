<?php
require 'vendor/autoload.php';
use Mailgun\Mailgun;

class MS_User_Email {
    protected  $mailer;
    protected  $email_model;
    public $email;
    private $tempPassword;
    private $passwordResetUrl;

    /**
     * @param $email string
     */
    public function __construct($email) {
        $this->mailer = new MS_Mailer();
        $this->email_model = new MS_Mail($email, null, null);
        $this->email = $email;
    }

    /**
     * register local seller if username does not
     * exist. Send a registration email
     */
    public function RegisterLocalSellerUser() {
        if(!$this->UserNameExists()) {
            $this->CreateLocalSeller();
            $this->sendRegistrationEmail();
        }
    }

    /**
     * send registration email
     */
    private function sendRegistrationEmail() {
        $this->email_model->to = $this->email;
        $new_user_registration_body = variable_get('ms_email_new_user_registration_body', array('value' => '', 'format' => NULL));
        $this->email_model->subject = t(variable_get('ms_email_new_user_registration_subject'), array('!email' => $this->email));

        $this->email_model->html = t($new_user_registration_body['value'],
            array('!one-time-sign-in-url' => $this->passwordResetUrl, '!email' => $this->email, '!password' => $this->tempPassword));
        $this->mailer->send($this->email_model);
    }

    /**
     * create a new authenticated user account
     * @throws Exception
     */
    private function CreateLocalSeller() {
        //This will generate a random password, you could set your own here
        $password = user_password(8);

        //set up the user fields
        $fields = array(
            'name' => $this->email,
            'mail' => $this->email,
            'pass' => $password,
            'status' => 1,
            'init' => 'email address',
            'roles' => array(
                DRUPAL_AUTHENTICATED_RID => 'authenticated user',
            ),
        );
        try {
            //the first parameter is left blank so a new user is created
            $account = user_save('', $fields);
        } catch(Exception $e) {
            throw new Exception("account creation error" . $e->getMessage());
        }
        // If you want to send the welcome email, use the following code

        // Manually set the password so it appears in the e-mail.
        $account->password = $fields['pass'];

        $this->tempPassword = $account->password;
        $this->passwordResetUrl = user_pass_reset_url($account);
    }
    /**
     * check if username already exists
     * @return bool - true if exists false if not
     */
    public function UserNameExists() {
        $result = false;
        $query = db_query("
            select * from users as u
            where u.name = '{$this->email}';
        ");

        $users = $query->fetchAll();
        if(count($users) > 0) {
            $result = true;
        }
        return $result;
    }
}
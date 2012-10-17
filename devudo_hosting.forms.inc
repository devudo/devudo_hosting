<?php

/**
 * @file devudo_hosting.forms.inc
 */

define('SECRET_PASSWORD', 'drupaltogo');
define('DEVMASTER_PLATFORM_NID', 187);
define('DEVMASTER_PROFILE_NID', 233);

/**
 * Implements hook_form_alter() for hosting_signup_form.
 */
function devudo_hosting_form_hosting_signup_form_alter(&$form, &$form_state) {
  global $user;
  drupal_set_title(t('Sign Up'));

  // Detect secret password
  if (!isset($_SESSION['secret_password']) || $_SESSION['secret_password'] != SECRET_PASSWORD){
    unset($form['site']);
    unset($form['client']);
    unset($form['#submit']);
    unset($form['#validate']);

    $form['password'] = array(
      '#title' => "What's the Password?",
      '#type' => 'password',
    );

    $form['#submit'][] = 'devudo_hosting_signup_password_validate';

    $form['submit']['#value'] = 'Open Sesame';
    $form['submit']['#weight'] = 10;
    return;
  }

  // Detect already signed up
  /*
  if ($_SESSION['signed_up']){
    drupal_set_message(t('You have already signed up.  Check your email for further instructions.'));
    drupal_goto('<front>');
  }
  */

  // Change "title" field
  $form['site']['title']['#title'] = t('Account Name');
  $form['site']['title']['#field_suffix'] = '.devudo.com';
  $form['site']['title']['#size'] = 16;
  $form['site']['title']['#element_validate'] = array('devudo_hosting_signup_form_domain_validate');
  $form['site']['title']['#description'] = t('Enter your account name, which will also be the URL for your your DevShop.');

  // Hard code platform & profile
  $form['site']['platform']['#default_value'] = $form['site']['platform']['#value'] = DEVMASTER_PLATFORM_NID;
  $form['site']['profile']['#default_value'] = $form['site']['profile']['#value'] = DEVMASTER_PROFILE_NID;
  $form['site']['platform']['#type'] =  $form['site']['profile']['#type'] = 'value';

  // Hide Language and DB server
  $form['site']['site_language']['#type'] = $form['site']['db_server']['#type'] = 'value';

  // Hide client and make it the same as title
  $form['client']['client_name']['#type'] ='value';
  $form['client']['client_name']['#required'] = FALSE;

  // Add our own validation
  $form['#validate'][] = 'devudo_hosting_signup_form_validate';
}

/**
 * Form validation for hosting_signup_form
 */
function devudo_hosting_signup_form_validate($form, &$form_state) {
  $client_name = trim($form_state['values']['title']);
dsm($form_state['values']);
  // Set client name
  form_set_value($form['client']['client_name'], $client_name, $form_state);
  form_set_value($form['client']['uname'], $client_name, $form_state);
  
  // Set domain name
  form_set_value($form['site']['title'], $client_name . '.devudo.com', $form_state);
  
  // Redirect to home
  $form_state['redirect'] = '<front>';
  
  // set a session var preventing double signup.
  $_SESSION['signed_up'] = TRUE;
}

/**
 * Form submission for hosting_signup_form
 */
function devudo_hosting_signup_password_validate($form, &$form_state) {
  if (trim($form_state['values']['password']) != SECRET_PASSWORD){
    form_set_error('password', t('WRONG!') . '<br><img src="/sites/all/modules/devudo_hosting/wrong.jpg"><br>' . t('Please try again.'));
  } else {
    $_SESSION['secret_password'] = SECRET_PASSWORD;
  }
}
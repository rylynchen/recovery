<?php

/**
 * Display recovery admin page
 */
function recovery_manage() {
  $view = views_get_view('recovery', 'default');
  $view->override_url = $_GET['q'];
  return $view->preview();
}

/**
 * Returns a recovery submission form.
 */
function recovery_add() {
  $recovery = entity_get_controller('recovery')->create();
  $output = drupal_get_form('recovery_form', $recovery);
  return $output;
}

/**
 * recovery view page.
 */
function recovery_page_view($recovery, $view_mode = 'full') {
  $build = $recovery->view($view_mode);
  if ($view_mode == 'full') {
    $build['recovery'][$recovery->rid]['uid'] = array(
      '#markup' => '<div class="field field-type-text field-label-above"><div class="field-label">Uid:</div><div class="field-items"><div class="field-item even">'.filter_xss($recovery->uid).'</div></div></div>',
    );
    $build['recovery'][$recovery->rid]['type'] = array(
      '#markup' => '<div class="field field-type-text field-label-above"><div class="field-label">Type:</div><div class="field-items"><div class="field-item even">'.filter_xss($recovery->type).'</div></div></div>',
    );
    $build['recovery'][$recovery->rid]['code'] = array(
      '#markup' => '<div class="field field-type-text field-label-above"><div class="field-label">Code:</div><div class="field-items"><div class="field-item even">'.filter_xss($recovery->code).'</div></div></div>',
    );
    $build['recovery'][$recovery->rid]['created'] = array(
      '#markup' => '<div class="field field-type-text field-label-above"><div class="field-label">Created:</div><div class="field-items"><div class="field-item even">'.filter_xss($recovery->created).'</div></div></div>',
    );
    $build['recovery'][$recovery->rid]['expiry'] = array(
      '#markup' => '<div class="field field-type-text field-label-above"><div class="field-label">Expiry:</div><div class="field-items"><div class="field-item even">'.filter_xss($recovery->expiry).'</div></div></div>',
    );
    $build['recovery'][$recovery->rid]['status'] = array(
      '#markup' => '<div class="field field-type-text field-label-above"><div class="field-label">Status:</div><div class="field-items"><div class="field-item even">'.filter_xss($recovery->status).'</div></div></div>',
    );
  }
  return $build;
}

/**
 * recovery edit page.
 */
function recovery_page_edit($recovery) {
  return drupal_get_form('recovery_form', $recovery);
}

/**
 * Confirm form for deleting a recovery.
 */
function recovery_delete_confirm_form($form, &$form_state, $recovery) {
  $form_state += array('recovery' => $recovery);
  $confirm_question = t('Are you sure you want to delete recovery for Rid : %rid?', array('%rid' => $recovery->rid));
  return confirm_form($form, $confirm_question, 'recovery/'.$recovery->rid);
}

/**
 * recovery delete confirm form submit handler.
 */
function recovery_delete_confirm_form_submit($form, &$form_state) {
  $recovery = $form_state['recovery'];
  $recovery->delete();
  drupal_set_message(t('Delete recovery for Rid: %rid.', array('%rid' => $recovery->rid)));
  $form_state['redirect'] = 'admin/structure/recovery';
}

/**
 * Page to recovery find.
 */
function recovery_find() {
  $form = array();
  $form['#attributes'] = array('class' => array('recovery_find'));
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => NULL,
  );
  $form['type'] = array(
    '#type' => 'radios',
    '#title' => t('方式'),
    '#options' => array('mail' => t('邮箱'), 'mobile' => t('手机')),
  );
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('邮箱地址'),
    '#description' => t('请输入账号注册时的邮箱地址'),
    '#states' => array(
      'visible' => array(
        'input[name="type"]' => array('value' => 'mail'),
      ),
    ),
  );
  $form['mobile'] = array(
    '#type' => 'textfield',
    '#title' => t('手机号码'),
    '#description' => t('请输入账号注册时的手机号码'),
    '#size' => 20,
    '#states' => array(
      'visible' => array(
        'input[name="type"]' => array('value' => 'mobile'),
      ),
    ),
    '#suffix' => '<div class="ajax_callback ajax_callback_mobile"></div><div class="clear"></div>',
  );
  $form['send_msg'] = array(
    '#type' => 'submit',
    '#value' => t('发送短信验证码'),
    '#submit' => array('recovery_find_send_msg_submit'),
    '#suffix' => '<div class="clear"></div>',
    '#validate' => array(),
    '#attributes' => array('class' => array('send_msg')),
    '#limit_validation_errors' => array(),
    '#ajax'=> array(
      'callback'=>'recovery_find_send_msg_callback',
      'progress' => array(
        'type' => 'throbber',
        'message' => '',
      ),
    ),
    '#states' => array(
      'visible' => array(
        'input[name="type"]' => array('value' => 'mobile'),
      ),
    ),
  );
  $form['msg_code'] = array(
    '#type' => 'textfield',
    '#title' => t('短信验证码'),
    '#size' => 20,
    '#description' => t('不区分大小写'),
    '#states' => array(
      'visible' => array(
        'input[name="type"]' => array('value' => 'mobile'),
      ),
    ),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('提交')
  );
  $form['#attached']['css'][] = drupal_get_path('module', 'recovery').'/recovery.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'recovery').'/recovery.js';
  $form['#validate'][] = 'recovery_find_validate';
  $form['#submit'][] = 'recovery_find_submit';
  return $form;
}

/**
 * Orth user register form send msg submit handler.
 */
function recovery_find_send_msg_submit($form, &$form_state) {
  $mobile = trim($form_state['input']['mobile']);
  $form_state['storage']['send_msg'] = array();
  if (empty($mobile)) {
    $form_state['storage']['send_msg']['send'] = FALSE;
    $form_state['storage']['send_msg']['callback']['img']['path'] = 'misc/watchdog-error.png';
    $form_state['storage']['send_msg']['callback']['img']['alt'] = t('不可用');
    $form_state['storage']['send_msg']['callback']['msg']['text'] = t('手机号不能为空');
  }
  else {
    if (isPhone($mobile)) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'user')
        ->entityCondition('bundle', 'user')
        ->fieldCondition('field_or_phone', 'value', $mobile);
      $result = $query->execute();
      if (isset($result['user'])) {
        form_set_value($form['uid'], reset($result['user'])->uid, $form_state);
        $form_state['storage']['send_msg']['send'] = TRUE;
        $form_state['storage']['send_msg']['callback']['img']['path'] = 'misc/watchdog-ok.png';
        $form_state['storage']['send_msg']['callback']['img']['alt'] = t('可用');
        $form_state['storage']['send_msg']['callback']['msg']['text'] = t('已发送验证码');
      }
      else {
        $form_state['storage']['send_msg']['send'] = TRUE;
        $form_state['storage']['send_msg']['callback']['img']['path'] = 'misc/watchdog-error.png';
        $form_state['storage']['send_msg']['callback']['img']['alt'] = t('不可用');
        $form_state['storage']['send_msg']['callback']['msg']['text'] = t('无效的手机号码');
      }
    }
    else {
      $form_state['storage']['send_msg']['send'] = FALSE;
      $form_state['storage']['send_msg']['callback']['img']['path'] = 'misc/watchdog-error.png';
      $form_state['storage']['send_msg']['callback']['img']['alt'] = t('不可用');
      $form_state['storage']['send_msg']['callback']['msg']['text'] = t('无效的手机号');
    }
  }

  if ($form_state['storage']['send_msg']['send']) {
    $account = $form_state['account']['#value'];
    $code = recovery_generate_code();
    $tokens = recovery_settings_tokens();
    $settings = recovery_settings();
    $site = variable_get('site_name', '');
    $msg = str_replace(
              array(
                $tokens['mobile']['val']['code'],
                $tokens['mobile']['val']['site'],
                $tokens['mobile']['val']['username']
              ), 
              array(
                $code, 
                $site, 
                $account->name
              ), 
              $settings->mobile_body
            );
    or_base_send_sms($mobile, $msg);
    $form_state['storage']['send_msg']['code'] = $code;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Orth user register form send msg submit ajax callback.
 */
function recovery_find_send_msg_callback($form, &$form_state) {
  $commands = array();
  $img = theme('image', array('path' => $form_state['storage']['send_msg']['callback']['img']['path'], 'width' => 18, 'height' => 18, 'alt' => $form_state['storage']['send_msg']['callback']['img']['alt']));
  $msg = '<span>'.$form_state['storage']['send_msg']['callback']['msg']['text'].'</span>';
  $html = $img.$msg;
  $commands[] = ajax_command_html('.ajax_callback_mobile', $html);
  if ($form_state['storage']['send_msg']['send']) {
    $commands[] = array(
      'command' => 'recoverySendMsgLock',
      'data' => array('lock_time' => __OR_SEND_PHONE_MSG_LOCK_TIME__),
    );
  }
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Form or_recovery_get_back_form validate handler.
 */
function recovery_find_validate($form, &$form_state) {
  $type = $form_state['values']['type'];
  if ($type == 'mail') {
    $mail = trim($form_state['values']['mail']);
    if (empty($mail)) {
      form_set_error('mail', t('请输入邮箱地址'));
    }
    elseif (!isEmail($mail)) {
      form_set_error('mail', t('无效的邮箱地址'));
    }
    else{
      $uid = db_select('users', 'u')->condition('u.mail', $mail)->fields('u', array('uid'))->execute()->fetchField();
      if ($uid) {
        form_set_value($form['uid'], $uid, $form_state);
      }
      else {
        form_set_error('mail', t('无效的邮箱地址'));
      }
    }
  }
  elseif ($type == 'mobile') {
    $mobile = trim($form_state['values']['mobile']);
    $code = trim($form_state['values']['msg_code']);
    if (empty($mobile)) {
      form_set_error('mobile', t('请输入手机号码'));
    }
    elseif (!isPhone($mobile)) {
      form_set_error('mobile', t('无效的手机号码'));
    }
    else{
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'user')
        ->entityCondition('bundle', 'user')
        ->fieldCondition('field_or_phone', 'value', $mobile);
      $result = $query->execute();
      if (isset($result['user'])) {
        form_set_value($form['uid'], reset($result['user'])->uid, $form_state);
      }
      else {
        form_set_error('mobile', t('无效的手机号码'));
      }
    }

    if (empty($code)) {
      form_set_error('msg_code', t('请输入验证码'));
    }
    elseif (strtolower($code) != strtolower($form_state['storage']['send_msg']['code'])) {
      form_set_error('msg_code', t('验证码错误'));
    }
    else {
      form_set_value($form['msg_code'], $code, $form_state);
    }
  }
  else {
    form_set_error('type', t('请选择找回方式'));
  }
}

/**
 * Form or_recovery_get_back_form submit handler.
 */
function recovery_find_submit($form, &$form_state) {
  $type = $form_state['values']['type'];
  $tokens = recovery_settings_tokens();
  $settings = recovery_settings();
  $site = variable_get('site_name', '');
  $uid = $form_state['values']['uid'];
  $account = user_load($uid);
  // create recovery
  $recovery = recovery_create();
  $recovery->uid = $uid;
  $recovery->type = $type;
  $recovery->created = time();
  $recovery->status = 0;
  if ($type == 'mail') {
    global $base_url;
    $code = recovery_generate_code();
    $recovery->expiry = $recovery->created + $settings->mail_expiry * 60 * 60;
    $recovery->code = $code;
    $encode_str = or_base_encode_str($code);
    $url = $base_url.'/recovery/reset/'.$encode_str;
    $link = '<a target="_blank" href="'.$url.'">'.$url.'</a>';
    $mail = $form_state['values']['mail'];
    $from = variable_get('site_mail', ini_get('sendmail_from'));
    $title = str_replace(
              array(
                $tokens['mail']['val']['link'],
                $tokens['mail']['val']['site'],
                $tokens['mail']['val']['username']
              ), 
              array(
                $link, 
                $site, 
                $account->name
              ), 
              $settings->mail_title
            );
    $body = str_replace(
              array(
                $tokens['mail']['val']['link'],
                $tokens['mail']['val']['site'],
                $tokens['mail']['val']['username']
              ), 
              array(
                $link, 
                $site, 
                $account->name
              ), 
              $settings->mail_body
            );
    $param = array('email' => $mail, 'subject' => $title, 'message' => $body);
    $result = drupal_mail('recovery', 'recovery', $mail, language_default(), $param, $from, TRUE);
    if ($result['result']) {
      $msg = t('已发送找回密码邮件至 <b>%mail</b>，请查收。', array('%mail' => $form_state['values']['mail']));
      $status = 'status';
    }
    else{
      $msg = t('发送邮件失败[%code]，请联系管理人员', array('%code' => 'RV_00'));
      $status = 'error';
      watchdog('recovery', t('[RV_00] Send mail fail, mail:%mail,title:%title, body:%body', array('%mail' => $mail, '%title' => $title, '%body' => $body)), WATCHDOG_ERROR);
    }
    drupal_set_message($msg, $status);
  }
  else {
    $recovery->expiry = $recovery->created + $settings->mobile_expiry * 60 * 60;
    $recovery->code = $form_state['values']['msg_code'];
    $encode_str = or_base_encode_str($recovery->code);
    $form_state['redirect'] = 'recovery/reset/'.$encode_str;
  }
  recovery_save($recovery);
}

/**
 * Generate recovery code.
 */
function recovery_generate_code() {
  $exist = db_select('recovery', 'r')->fields('r', array('code'))->execute()->fetchCol();
  do{
    $code = user_password(8);
  }while(in_array($code, $exist));
  return $code;
}

/**
 * Recovery reset password form.
 */
function recovery_reset($form, &$form_state, $recovery) {
  $form['recovery'] = array(
    '#type' => 'value',
    '#value' => $recovery,
  );
  $form['pwd'] = array(
    '#type' => 'password',
    '#title' => t('新密码'),
  );
  $form['repwd'] = array(
    '#type' => 'password',
    '#title' => t('确认新密码'),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('提交'),
  );
  $form['#validate'][] = 'recovery_reset_validate';
  $form['#submit'][] = 'recovery_reset_submit';
  return $form;
}

/**
 * Form recovery_reset validate handler.
 */
function recovery_reset_validate($form, &$form_state) {
  $pwd = $form_state['values']['pwd'];
  $repwd = $form_state['values']['repwd'];
  // verify pwd
  if (empty($pwd)) {
    form_set_error('pwd', t('密码不能为空'));
  }
  else {
    if (__OR_USER_PWD_LENGTH_MIN__ && strlen($pwd) < __OR_USER_PWD_LENGTH_MIN__) {
      form_set_error('pwd', t('密码不能少于'.__OR_USER_PWD_LENGTH_MIN__.'位'));
    }
    else if (!__OR_USER_PWD_EASY__ && in_array($pwd, orth_register_easy_pwd())) {
      form_set_error('pwd', t('密码过于简单'));
    }
    elseif ($repwd != $pwd) {
      form_set_error('repwd', t('确认密码与密码不同'));
    }
  }
}

/**
 * Form recovery_reset submit handler.
 */
function recovery_reset_submit($form, &$form_state) {
  $recovery = $form_state['values']['recovery'];
  $recovery->status = 1;
  recovery_save($recovery);
  $account = user_load($recovery->uid);
  user_save($account, array('pass' => $form_state['values']['pwd']));
  or_login($account->uid);
  drupal_set_message(t('设置新密码成功'));
  $form_state['redirect'] = 'user';
}
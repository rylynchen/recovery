Drupal.behaviors.recovery = {
  attach: function(context, settings) {
    jQuery('form.recovery_find')
      .ajaxStart(function(){
          jQuery(this).submit(function() {
            return false;
          });
        })
       .ajaxStop(function() {
          jQuery(this).unbind('submit');
          jQuery(this).submit(function() {
            return true;
          });
        });
  }
}

Drupal.ajax.prototype.commands.recoverySendMsgLock = function(ajax, response, status)
{
  var wait = response.data.lock_time;
  jQuery('.send_msg').attr('disabled', 'disabled');
  var fresh = setInterval(function () {
    if (wait > 0) {
      jQuery('.send_msg').attr('value', Drupal.t(wait + '秒后可以重新发送'));
      wait -= 1;
    }
    else {
      window.clearInterval(fresh);
      jQuery('.send_msg').removeAttr('disabled').val(Drupal.t('发送短信验证码'));
    }
  }, 1000);
};
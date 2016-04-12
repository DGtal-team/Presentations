$.getJSON(
    'https://api.github.com/repos/DGtal-team/DGtal/contributors',
    function( data ) {
      $.each( data, function( key, user ) {
        if( user.id !== 3092978 && user.id !== 148100 ){
              var html = '<li class="avatar"><a href="' + user.html_url;
              html += '" title="@' + user.login + ' with ' + user.contributions;
                html += ' contributions">';
                html += '<img src="' + user.avatar_url + ' class="avatar" /></a><h1>'+user.login+'</h1></li>';
                $( '#team' ).append( html );
              }
        } );
    }
);

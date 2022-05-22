$(function () {

    let $preview = $('#map-preview'),
        $map_content = $('#map-contents');

    $preview.hide();

    $.getScript('/static/js/pixi.min.js')
    .done(function() {
        $.getScript('/static/js/game.js')
        .done(function() {
            let game = new Game();
            game.addToDOM($preview)
            game.readMap($map_content.replaceAll('\n', ''), map_char_to_texture);
            game.setupAnimation();

            // reveal the UI
            $preview.fadeIn('fast');
        });
    });

});

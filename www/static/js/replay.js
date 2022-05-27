let dump_data = undefined;
let current_turn = 0;
let pause = true;

$(function () {
    let $replay = $('#replay'),
        $playPause = $('#replay-playpause'),
        $previous = $('#replay-previous'),
        $next = $('#replay-next'),
        $turnLabel = $('#replay-turn-label'),
        $turnSlider = $('#replay-turn-slider');

    $replay.append('<div id="replay_view"></div>');
    let $replay_view = $("#replay_view");

    $turnSlider.attr('min', 0).attr('max', 400 - 1).val(0);

    $replay_view.hide();

    $.getScript('/static/js/pixi.min.js').done(function() {
        $.getScript('/static/js/game.js')
        .done(function() {
            let game = create_game();
            game.addToDOM($replay_view[0]);

            // Load dump
            const dump_url = `${window.location.href}dump`;
            fetch(dump_url).then(response => {
                if (response.status !== 200)
                    console.error('no dump found', response);
                response.text().then(data => {
                    data = data.substring(0, data.length - 1).replaceAll('\n', ',');
                    dump_data = JSON.parse(`{"dump": [${data}]}`)["dump"];
                    console.log('dump loaded');
                    game.setupGame(dump_data);
                });
            });

            // reveal the UI
            $replay_view.find('canvas').css({
                'display': 'block',
                'max-width': '100%',
                'max-height': '100%',
                'margin': 'auto',
            });
            $replay_view.fadeIn('fast');

            $previous[0].onclick = e => {
                if (current_turn == 0)
                    return;
                current_turn -= 1;
                $turnLabel.text(current_turn);
                game.jumpToRound(current_turn);
                console.log(dump_data);
            };

            $playPause[0].onclick = e => {
                game.startReplay(current_turn);
            };

            $next[0].onclick = e => {
                if (current_turn == 400)
                    return;
                current_turn += 1;
                $turnLabel.text(current_turn);
                game.jumpToRound(current_turn);
            };

            $turnSlider.change(function (e) {
                current_turn = parseInt($turnSlider.val());
                $previous.prop('disabled', current_turn <= 0);
                $next.prop('disabled', current_turn > 400);
                $turnLabel.text(current_turn);

                // Trigger update iff the event was trigger by the UI
                if (e.originalEvent)
                    game.jumpToRound(current_turn);
            });
        });
    });
});

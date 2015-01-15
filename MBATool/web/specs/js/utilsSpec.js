define(['js/utils'], function(Utils) {

    describe('decoratePitcher', function() {

        it('should add the cut and selected boolean fields', function() {
            var player = {};

            Utils.decoratePitcher(player, null);

            expect(player.isCut     ).toEqual(false);
            expect(player.isSelected).toEqual(false);
        });

        it('should calculate the player rating', function() {
            var player = { speed: 5, control: 4, fatigue: 8 };

            Utils.decoratePitcher(player, null);

            expect(player.rating).toEqual(17);
        });
    });
});

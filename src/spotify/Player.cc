#include <libspotify/api.h>
#include "Player.h"
#include "Track.h"
#include "PlaylistContainer.h"
#include "../SpotifyService/SpotifyService.h"

extern SpotifyService* spotifyService;
extern PlaylistContainer* playlistContainer;

Handle<Value> Player::pause(const Arguments& args) {
	return Player::simpleCall(args, &Player::spotifyPause);
}

Handle<Value> Player::stop(const Arguments& args) {
	return Player::simpleCall(args, &Player::spotifyStop);
}

Handle<Value> Player::resume(const Arguments& args) {
	return Player::simpleCall(args, &Player::spotifyResume);
}

Handle<Value> Player::play(const Arguments& args) {
	Player* player = node::ObjectWrap::Unwrap<Player>(args.This());

	int playlistId = args[0]->ToInteger()->Value();
	player->currentTrack = args[1]->ToInteger()->Value();

  player->playlist = playlistContainer->getPlaylists()[playlistId];
	return Player::simpleCall(args, &Player::spotifyPlay);
}

void Player::spotifyPause() {
	sp_session_player_play(spotifyService->getSpotifySession(), 0);
	isPaused = true;
}

void Player::spotifyResume() {
	if(isPaused) {
		sp_session_player_play(spotifyService->getSpotifySession(), 1);
		isPaused = false;
	}
}

void Player::spotifyStop() {
	sp_session_player_unload(spotifyService->getSpotifySession());
}

void Player::spotifyPlay() {
  if(playlist != 0) {
  	Track* track = playlist->getTracks()[currentTrack];
  	sp_session_player_unload(spotifyService->getSpotifySession());
    sp_session_player_load(spotifyService->getSpotifySession(), track->spotifyTrack);
    sp_session_player_play(spotifyService->getSpotifySession(), 1);
  }
}

void Player::nextTrack() {
	if(playlist != 0) {
		currentTrack++;
		Track* track = playlist->getTracks()[currentTrack];
		if( currentTrack < (int)playlist->getTracks().size()) {
			sp_session_player_unload(spotifyService->getSpotifySession());
			sp_session_player_load(spotifyService->getSpotifySession(), track->spotifyTrack);
    	sp_session_player_play(spotifyService->getSpotifySession(), 1);
		}
	}
}

void Player::init(Handle<Object> target) {
	HandleScope scope;
	Local<FunctionTemplate> constructorTemplate = FunctionTemplate::New();
	constructorTemplate->SetClassName(String::NewSymbol("Player"));
	constructorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
	
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "play", play);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "pause", pause);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "resume", resume);
	NODE_SET_PROTOTYPE_METHOD(constructorTemplate, "stop", stop);
  constructor = Persistent<Function>::New(constructorTemplate->GetFunction());
  scope.Close(Undefined());
}
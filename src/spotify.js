/**
The MIT License (MIT)

Copyright (c) <2013> <Moritz Schulze>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
**/

var _spotify = require('./nodespotify');
var metadataUpdater = require('./metadataUpdater');

function addMethodsToPrototypes(sp) {
  sp.internal.protos.Playlist.prototype.getTracks = function() {
    var out = new Array(this.numTracks);
    for(var i = 0; i < this.numTracks; i++) {
      out[i] = this.getTrack(i);
    }
    return out;
  }
}

var beefedupSpotify = function(options) {
  var spotify = _spotify(options);
  addMethodsToPrototypes(spotify);
  spotify.version = '0.5.4';

  spotify.on = function(callbacks) {
    if(callbacks.metadataUpdated) {
      var userCallback = callbacks.metadataUpdated;
      callbacks.metadataUpdated = function() {
        userCallback();
        metadataUpdater.metadataUpdated();
      }
    } else {
      callbacks.metadataUpdated = metadataUpdater.metadataUpdated;
    }
    spotify._on(callbacks);
  }

  spotify.waitForLoaded = metadataUpdater.waitForLoaded;
  return spotify;
}

module.exports = beefedupSpotify;

import { Link } from 'react-router-dom'

export default function Home() {
  return (
    <main className="min-h-screen bg-zinc-950 text-white">

      {/* Hero Section */}
      <section className="px-8 py-32 text-center bg-gradient-to-b from-zinc-900 to-zinc-950">
        <h1 className="text-5xl sm:text-6xl font-bold tracking-tight">
          Maryn Launcher
        </h1>

        <p className="mt-6 text-lg sm:text-xl text-zinc-400 max-w-xl mx-auto">
          A lightweight, open-source Minecraft Java Edition launcher with offline account support.
        </p>

        <div className="mt-10 flex justify-center gap-4">
          <Link
            to="/download"
            className="bg-blue-600 hover:bg-blue-500 text-white px-6 py-3 rounded-lg font-semibold transition-colors"
          >
            Download
          </Link>

          <Link
            to="/community"
            className="border border-zinc-600 px-6 py-3 rounded-lg hover:bg-zinc-800 transition-colors"
          >
            Community
          </Link>
        </div>
      </section>

      {/* Features Section */}
      <section className="px-8 py-20 max-w-4xl mx-auto">
        <h2 className="text-3xl font-bold text-center mb-12">
          Why Maryn Launcher?
        </h2>

        <div className="grid grid-cols-1 sm:grid-cols-3 gap-6">

          <div className="bg-zinc-900 rounded-xl p-6 border border-zinc-800">
            <h3 className="text-lg font-semibold mb-2">Offline Support</h3>
            <p className="text-zinc-400 text-sm">
              Play Minecraft without a Microsoft account using offline/cracked mode.
            </p>
          </div>

          <div className="bg-zinc-900 rounded-xl p-6 border border-zinc-800">
            <h3 className="text-lg font-semibold mb-2">Lightweight</h3>
            <p className="text-zinc-400 text-sm">
              Built with C++ and Qt6. No Electron, no bloat — just a fast native app.
            </p>
          </div>

          <div className="bg-zinc-900 rounded-xl p-6 border border-zinc-800">
            <h3 className="text-lg font-semibold mb-2">Open Source</h3>
            <p className="text-zinc-400 text-sm">
              Fully open source on GitHub. Inspect, contribute, or fork it freely.
            </p>
          </div>

        </div>
      </section>

    </main>
  )
}